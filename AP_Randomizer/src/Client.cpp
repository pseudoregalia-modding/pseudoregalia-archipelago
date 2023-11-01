#pragma once
#pragma comment (lib, "crypt32") // Include crypt32.lib
#pragma warning(disable: 4267) // Disable conversion warnings from websocketpp and apclientpp
#pragma warning(disable: 4996) // Disable deprecated warnings from old asio version and apclientpp
#pragma warning(disable: 26495) // Disable uninitialized warnings from asio, websocketpp, and apclientpp
#pragma warning(disable: 26439) // Disable noexcept warnings from asio

// Define asio standalone even though we need boost installed for certain dependencies,
// because otherwise we end up with repeated definitions.
// Honestly this library confuses the hell out of me.
#define ASIO_STANDALONE
#define BOOST_ALL_NO_LIB // Allows us to use asio boost as header-only
#define APCLIENT_DEBUG
#include "Archipelago.h"
#include "apclient.hpp"
#include "apuuid.hpp"
#include "GameData.hpp"
#include "Engine.hpp"
#include "Client.hpp"
#include "Logger.hpp"
#include "Timer.hpp"

namespace Client {
    // Private members
    namespace {
        typedef nlohmann::json json;
        typedef APClient::State ConnectionStatus;
        void ReceiveItems(const std::list<APClient::NetworkItem>&);
        void PrintJsonMessage(const APClient::PrintJSONArgs&);
        void ReceiveDeathLink(const json&);

        // I don't think a mutex is required here because apclientpp locks the instance during poll().
        // If people report random crashes, especially when disconnecting, I'll revisit it.
        APClient* ap;
        AP_ConnectionStatus connection_status;
        std::string uri;
        const std::string uuid = ap_get_uuid("Mods/AP_Randomizer/dlls/uuid");
        const std::string game_name = "Pseudoregalia";
        const int max_connection_retries = 3;
        int connection_retries = 0;
        int slot_number;
        bool death_link_locked;
        const float death_link_timer_seconds(4.0f);
    } // End private members

    void Client::Connect(const std::string new_uri, const std::string slot_name, const std::string password) {
        // Nuke any existing client in case uri needs to change.
        if (ap != nullptr) {
            delete ap;
        }
        GameData::Initialize();
        uri = new_uri;
        ap = new APClient(uuid, game_name, uri); // TODO: add cert store
        connection_retries = 0;

        // Print feedback to the player so they know the connect command went through.
        std::string connect_message(
            "Attempting to connect to " + new_uri
            + " with name " + slot_name);
        Log(connect_message, LogType::System);

        // The Great Wall Of Callbacks
        {
            // Executes when the server sends room info; attempts to connect the player.
            ap->set_room_info_handler([slot_name, password]() {
                Log("Received room info");
                int items_handling = 0b111;
                APClient::Version version{ 0, 6, 2 };
                ap->ConnectSlot(slot_name, password, items_handling, {}, version);
                });

            // Executes on successful connection to slot.
            ap->set_slot_connected_handler([](const json& slot_data) {
                Log("Connected to slot");
                for (json::const_iterator iter = slot_data.begin(); iter != slot_data.end(); iter++) {
                    GameData::SetOption(iter.key(), iter.value());
                    if (iter.key() == "death_link" || iter.value() > 0) {
                        ap->ConnectUpdate(false, 0, true, std::list<std::string> {"DeathLink"});
                    }
                }
                Engine::SpawnCollectibles();
                connection_retries = 0;
                });

            // Executes whenever a socket error is detected.
            // We want to only print an error after exactly X attempts.
            ap->set_socket_error_handler([](const std::string& error) {
                Log("Socket error: " + error);
                if (connection_retries == max_connection_retries) {
                    if (ap->get_player_number() >= 0) { // Seed is already in progress
                        Log(L"Lost connection with the server. Attempting to reconnect...", LogType::System);
                    }
                    else { // Attempting to connect to a new room
                        Log(L"Could not connect to the server. Please double-check the address and ensure the server is active.", LogType::System);
                    }
                }
                connection_retries++;
                });

            // Executes when the server refuses slot connection.
            ap->set_slot_refused_handler([](const std::list<std::string>& reasons) {
                std::string advice;
                if (std::find(reasons.begin(), reasons.end(), "InvalidSlot") != reasons.end()
                    || std::find(reasons.begin(), reasons.end(), "InvalidPassword") != reasons.end()) {
                    advice = "Please double-check your slot name and password.";
                }
                if (std::find(reasons.begin(), reasons.end(), "IncompatibleVersion") != reasons.end()) {
                    advice = "Please double-check your client version.";
                }
                Log("Could not connect to the server. " + advice, LogType::System);
                });

            ap->set_items_received_handler(&ReceiveItems);
            ap->set_location_checked_handler(&GameData::CheckLocations);
            ap->set_print_json_handler(&PrintJsonMessage);
            ap->set_bounced_handler(&ReceiveDeathLink);
        } // End callbacks
    }

    void Client::Disconnect() {
        if (ap == nullptr) {
            Log("You are not currently connected.", LogType::System);
            return;
        }
        delete ap;
        ap = nullptr;
        Log("Disconnected from " + uri, LogType::System);
    }

    void Client::SendCheck(int64_t id, std::wstring current_world) {
        // TODO: Consider refactoring to queue location ids as an actual list
        std::list<int64_t> id_list{ id };
        Log(L"Sending check with id " + std::to_wstring(id));
        ap->LocationChecks(id_list);
    }
    
    // Sends game completion flag to Archipelago.
    void Client::CompleteGame() {
        if (ap == nullptr) {
            // TODO: queue this if the player forgets to connect before winning
            return;
        }
        ap->StatusUpdate(APClient::ClientStatus::GOAL);

        // Send a key to datastorage upon game completion for PopTracker integration.
        std::string key =
            "Pseudoregalia - Team " + std::to_string(ap->get_team_number())
            + " - Player " + std::to_string(ap->get_player_number())
            + " - Game Complete";
        json default_value{ 0 };
        std::list<APClient::DataStorageOperation> filler_operations{ APClient::DataStorageOperation{ "add", default_value  } };
        ap->Set(key, default_value, true, filler_operations);
    }

    void Client::PollServer() {
        if (ap == nullptr) {
            return;
        }
        ap->poll();
    }

    void Client::SendDeathLink() {
        if (ap == nullptr
        || !GameData::GetOptions().at("death_link")
        || death_link_locked) {
            return;
        }

        // TODO: more of these.
        // I don't think there's an easy to way to get cause of death from within the game unfortunately.
        std::string funny_message(ap->get_slot());
        funny_message.append(" wished death upon their friends.");
        json data{
            {"time", ap->get_server_time()},
            {"cause", funny_message},
            {"source", ap->get_slot()},
        };
        ap->Bounce(data, {}, {}, { "DeathLink" });
        Log(L"Sending death to your friends ♥", LogType::Popup);
        Log("Sending bounce: " + data.dump());
        Timer::RunTimerInGame(death_link_timer_seconds, &death_link_locked);
    }


    // Private functions
    namespace {
        void PrintJsonMessage(const APClient::PrintJSONArgs& args) {
            std::string message_text = ap->render_json(args.data);
            Log(message_text, LogType::Popup);
        }

        void ReceiveItems(const std::list<APClient::NetworkItem>& items) {
            for (const auto& item : items) {
                int64_t id = item.item;
                Log(L"Receiving item with id " + std::to_wstring(id));
                GameData::ReceiveItem(id);
                Engine::SyncItems();
            }
        }

        void ReceiveDeathLink(const json& data) {
            Log("Receiving bounce: " + data.dump());
            if (ap == nullptr
                || !GameData::GetOptions().at("death_link")
                || death_link_locked) {
                return;
            }

            if (!data.contains("data")) {
                // Should only execute if the received death link data was not properly filled out.
                Log("You were assassinated by a mysterious villain...", LogType::Popup);
                Engine::VaporizeGoat();
                Timer::RunTimerInGame(death_link_timer_seconds, &death_link_locked);
                return;
            }
            json details = data["data"];
            std::string funny_message;

            if (details.contains("cause")) {
                std::string cause(details["cause"]);
                Log(cause, LogType::Popup);
            }
            else if (details.contains("source")) {
                std::string source(details["source"]);
                Log("You were brutally murdered by " + source + ".", LogType::Popup);
            }
            else {
                // Should only execute if the received death link data was not properly filled out.
                Log("You were assassinated by a mysterious villain...", LogType::Popup);
            }
            Engine::VaporizeGoat();
            Timer::RunTimerInGame(death_link_timer_seconds, &death_link_locked);
        }
    } // End private functions
}