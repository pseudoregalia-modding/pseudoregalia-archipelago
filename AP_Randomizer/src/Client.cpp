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
        bool ConnectionStatusChanged();
        void SetSlotNumber(int);
        void SetSplitKicks(int);
        void ReceiveDeathLink();
        void ConnectionTimerExpired();
        void ConnectToSlot();

        // I don't think a mutex is required here because apclientpp locks the instance during poll().
        // If people report random crashes, especially when disconnecting, I'll revisit it.
        APClient* client;
        AP_ConnectionStatus connection_status;
        std::string uri;
        std::string uuid;
        std::string slot_name;
        std::string password;
        const std::string game_name = "Pseudoregalia";
        const std::chrono::seconds connection_timer(15);
        int slot_number;
        bool death_link_locked;
        const float death_link_timer_seconds(4.0f);
    } // End private members

    /*
    TODO:
    - completing game
    - sending game completion flag to datastorage
    - setting options from slot data
    - death link
    */
    void Client::Connect(const char* new_ip, const char* new_slot_name, const char* new_password) {
        // Initialize game state.
        GameData::Initialize();
        uuid = ap_get_uuid("Mods/AP_Randomizer/dlls/uuid");
        uri = new_ip;
        slot_name = new_slot_name;
        password = new_password;

        client = new APClient(uuid, game_name, uri); // TODO: add cert store
        client->set_room_info_handler(&ConnectToSlot);
        client->set_items_received_handler(&ReceiveItems);
        client->set_location_checked_handler(&GameData::CheckLocations);
        client->set_slot_connected_handler(&OnSlotConnected);
        client->set_print_json_handler(&PrintJsonMessage);

        // Print feedback to the player so they know the connect command went through.
        std::string connect_message = "Attempting to connect to ";
        connect_message.append(new_ip);
        connect_message += " with name ";
        connect_message.append(new_slot_name);
        Logger::Log(connect_message, Logger::LogType::System);
        Timer::RunTimerRealTime(connection_timer, &ConnectionTimerExpired);
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
        //Logger::Log(std::to_wstring(id_list.size()), Logger::LogType::System);
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

        // Send a key to datastorage upon game completion for PopTracker integration
        // I get the slot number by storing it in slot data on generation but I'm pretty sure there's a less dumb way.
        AP_SetServerDataRequest completion_flag;
        AP_DataStorageOperation operation;
        int filler_value = 0;
        operation.operation = "add";
        operation.value = &filler_value;
        completion_flag.key = "Pseudoregalia - Player " + std::to_string(slot_number) + " - Game Complete";
        completion_flag.type = AP_DataType::Int;
        completion_flag.want_reply = true;
        completion_flag.operations.push_back(operation);
        AP_SetServerData(&completion_flag);
    }

    void Client::PollServer() {
        if (client == nullptr) {
            return;
        }
        client->poll();

        if (ConnectionStatusChanged()) {
            if (connection_status == AP_ConnectionStatus::ConnectionRefused) {
                Logger::Log(L"The server refused the connection. Please double-check your connection info and client version, and restart the game.", Logger::LogType::System);
            }
        }
    }

    void Client::SendDeathLink() {
        if (death_link_locked) {
            return;
        }
        Logger::Log(L"Sending death link");
        AP_DeathLinkSend();
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
            if (client->ConnectSlot(name, password, items_handling, {}, version)) {
                Logger::Log("Connected!", Logger::LogType::System);
            }
            else {
                Logger::Log("Failed to connect...", Logger::LogType::System);
            }
        }

        void PrintJsonMessage(const APClient::PrintJSONArgs& args) {
            std::string message_text = client->render_json(args.data);
            Logger::Log(message_text, Logger::LogType::Popup);
        }

        void OnSlotConnected(const json& slot_data) {
            Engine::SpawnCollectibles();
            for (json::const_iterator iter = slot_data.begin(); iter != slot_data.end(); iter++) {
                GameData::SetOption(iter.key(), iter.value());
            }
        }

        void SetSlotNumber(int num) {
            slot_number = num;
        }

        void SetSplitKicks(int is_true) {
            GameData::SetOption("split_sun_greaves", is_true);
        }

        void ReceiveItems(const std::list<APClient::NetworkItem>& items) {
            for (const auto& item : items) {
                int64_t id = item.item;
                Logger::Log(L"Receiving item with id " + std::to_wstring(id));
                GameData::ReceiveItem(id);
                Engine::SyncItems();
            }
        }

        void ReceiveDeathLink() {
            if (death_link_locked) {
                return;
            }
            Logger::Log(L"Receiving death link");
            Engine::VaporizeGoat();
            Timer::RunTimerInGame(death_link_timer_seconds, &death_link_locked);
        }

        bool ConnectionStatusChanged() {
            if (connection_status != AP_GetConnectionStatus()) {
                connection_status = AP_GetConnectionStatus();
                return true;
            }
            return false;
        }

        // Prints an error if the connection timer expires with no connection having been established.
        void ConnectionTimerExpired() {
            if (AP_GetConnectionStatus() == AP_ConnectionStatus::Disconnected) {
                Logger::Log(L"Could not find the address entered. Please double-check your connection info and restart the game.", Logger::LogType::System);
            }
        }
    } // End private functions
}