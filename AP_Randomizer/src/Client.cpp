#pragma once
#pragma comment (lib, "crypt32") // Include crypt32.lib
#pragma warning(disable: 4267) // Disable conversion warnings from websocketpp and apclientpp
#pragma warning(disable: 4996) // Disable deprecated warnings from old asio version and apclientpp

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
        void OnSlotConnected(const json&);
        void PrintJsonMessage(const APClient::PrintJSONArgs&);
        void ReceiveDeathLink(const json&);
        void ConnectToSlot();

        // I don't think a mutex is required here because apclientpp locks the instance during poll().
        // If people report random crashes, especially when disconnecting, I'll revisit it.
        APClient* client;
        AP_ConnectionStatus connection_status;
        std::string uri;
        const std::string uuid = ap_get_uuid("Mods/AP_Randomizer/dlls/uuid");
        std::string slot_name;
        std::string password;
        const std::string game_name = "Pseudoregalia";
        bool file_active = false; // Used to determine how to phrase socket errors based on whether the player expects to be connected already.
        const int max_connection_retries = 3;
        int connection_retries = 0;
        int slot_number;
        bool death_link_locked;
        const float death_link_timer_seconds(4.0f);
    } // End private members

    void Client::Connect(const char* new_ip, const char* new_slot_name, const char* new_password) {
        // Ensure error messages work correctly even when the player doesn't fully disconnect first.
        file_active = false;
        connection_retries = 0;

        // Nuke any existing client in case uri needs to change.
        if (client != nullptr) {
            delete client;
        }

        GameData::Initialize();
        uri = new_ip;
        slot_name = new_slot_name;
        password = new_password;

        client = new APClient(uuid, game_name, uri); // TODO: add cert store
        client->set_room_info_handler(&ConnectToSlot);
        client->set_items_received_handler(&ReceiveItems);
        client->set_location_checked_handler(&GameData::CheckLocations);
        client->set_slot_connected_handler(&OnSlotConnected);
        client->set_print_json_handler(&PrintJsonMessage);
        client->set_bounced_handler(&ReceiveDeathLink);

        client->set_socket_error_handler([](const std::string& error) {
            // Only print a message after exactly X failed attempts.
            if (connection_retries == max_connection_retries) {
                // Change error message based on whether a seed is already active.
                if (file_active) {
                    Logger::Log(L"Lost connection with the server. Attempting to reconnect...", Logger::LogType::System);
                }
                else {
                    Logger::Log(L"Could not connect to the server. Please double-check the address and ensure the server is active.", Logger::LogType::System);
                }
            }

            connection_retries++;
            Logger::Log(error);
            return;
            });

        client->set_slot_refused_handler([](const std::list<std::string>& reasons) {
            std::string advice;
            if (std::find(reasons.begin(), reasons.end(), "InvalidSlot") != reasons.end()
                || std::find(reasons.begin(), reasons.end(), "InvalidPassword") != reasons.end()) {
                advice = "Please double-check your slot name and password.";
            }

            if (std::find(reasons.begin(), reasons.end(), "IncompatibleVersion") != reasons.end()) {
                advice = "Please double-check your client version.";
            }

            Logger::Log("Could not connect to the server. " + advice, Logger::LogType::System);
            });

        // Print feedback to the player so they know the connect command went through.
        std::string connect_message = "Attempting to connect to ";
        connect_message.append(new_ip);
        connect_message += " with name ";
        connect_message.append(new_slot_name);
        Logger::Log(connect_message, Logger::LogType::System);
    }

    void Client::Disconnect() {
        if (client == nullptr) {
            Logger::Log("You are not currently connected.", Logger::LogType::System);
            return;
        }
        delete client;
        client = nullptr;
        Logger::Log("Disconnected from " + uri, Logger::LogType::System);
    }

    void Client::SendCheck(int64_t id, std::wstring current_world) {
        // TODO: Consider refactoring to queue location ids as an actual list
        std::list<int64_t> id_list{ id };
        Logger::Log(L"Sending check with id " + std::to_wstring(id));
        client->LocationChecks(id_list);
    }
    
    // Sends game completion flag to Archipelago.
    void Client::CompleteGame() {
        if (client == nullptr) {
            // TODO: queue this if the player forgets to connect before winning
            return;
        }
        client->StatusUpdate(APClient::ClientStatus::GOAL);

        // Send a key to datastorage upon game completion for PopTracker integration.
        std::string key =
            "Pseudoregalia - Team " + std::to_string(client->get_team_number())
            + " - Player " + std::to_string(client->get_player_number())
            + " - Game Complete";
        json default_value{ 0 };
        std::list<APClient::DataStorageOperation> filler_operations{ APClient::DataStorageOperation{ "add", default_value  } };
        client->Set(key, default_value, true, filler_operations);
    }

    void Client::PollServer() {
        if (client == nullptr) {
            return;
        }
        client->poll();
    }

    void Client::SendDeathLink() {
        if (client == nullptr
        || !GameData::GetOptions().at("death_link")
        || death_link_locked) {
            return;
        }

        // TODO: more of these.
        // I don't think there's an easy to way to get cause of death from within the game unfortunately.
        std::string funny_message(client->get_slot());
        funny_message.append(" wished death upon their friends.");
        json data{
            {"time", client->get_server_time()},
            {"cause", funny_message},
            {"source", client->get_slot()},
        };
        client->Bounce(data, {}, {}, { "DeathLink" });
        Logger::Log(L"Sending death to your friends ♥", Logger::LogType::Popup);
        Logger::Log("Sending bounce: " + data.dump());
        Timer::RunTimerInGame(death_link_timer_seconds, &death_link_locked);
    }


    // Private functions
    namespace {
        void ConnectToSlot() {
            std::string name = slot_name;
            std::string password = password;
            int items_handling = 0b111;
            APClient::Version version{ 0, 6, 2 };

            // TODO: Make this feedback better
            Logger::Log("attempting to connect");
            client->ConnectSlot(name, password, items_handling, {}, version);
        }

        void PrintJsonMessage(const APClient::PrintJSONArgs& args) {
            std::string message_text = client->render_json(args.data);
            Logger::Log(message_text, Logger::LogType::Popup);
        }

        void OnSlotConnected(const json& slot_data) {
            file_active = true;
            for (json::const_iterator iter = slot_data.begin(); iter != slot_data.end(); iter++) {
                GameData::SetOption(iter.key(), iter.value());
                if (iter.key() == "death_link" || iter.value() > 0) {
                    client->ConnectUpdate(false, 0, true, std::list<std::string> {"DeathLink"});
                }
            }
            Engine::SpawnCollectibles();
        }

        void ReceiveItems(const std::list<APClient::NetworkItem>& items) {
            for (const auto& item : items) {
                int64_t id = item.item;
                Logger::Log(L"Receiving item with id " + std::to_wstring(id));
                GameData::ReceiveItem(id);
                Engine::SyncItems();
            }
        }

        void ReceiveDeathLink(const json& data) {
            Logger::Log("Receiving bounce: " + data.dump());
            if (client == nullptr
                || !GameData::GetOptions().at("death_link")
                || death_link_locked) {
                return;
            }

            if (!data.contains("data")) {
                // Should only execute if the received death link data was not properly filled out.
                Logger::Log("You were assassinated by a mysterious villain...", Logger::LogType::Popup);
                Engine::VaporizeGoat();
                Timer::RunTimerInGame(death_link_timer_seconds, &death_link_locked);
                return;
            }
            json details = data["data"];
            std::string funny_message;

            if (details.contains("cause")) {
                std::string cause(details["cause"]);
                Logger::Log(cause, Logger::LogType::Popup);
            }
            else if (details.contains("source")) {
                std::string source(details["source"]);
                Logger::Log("You were brutally murdered by " + source + ".", Logger::LogType::Popup);
            }
            else {
                // Should only execute if the received death link data was not properly filled out.
                Logger::Log("You were assassinated by a mysterious villain...", Logger::LogType::Popup);
            }
            Engine::VaporizeGoat();
            Timer::RunTimerInGame(death_link_timer_seconds, &death_link_locked);
        }
    } // End private functions
}