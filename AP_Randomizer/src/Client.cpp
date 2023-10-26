#pragma once
#include "Archipelago.h"
#include "GameData.hpp"
#include "Engine.hpp"
#include "Client.hpp"
#include "Logger.hpp"
#include "Timer.hpp"

namespace Client {
    // Private members
    namespace {
        void ReceiveItem(int64_t, bool);
        void CheckLocation(int64_t);
        bool ConnectionStatusChanged();
        void SetSlotNumber(int);
        void SetSplitKicks(int);
        void ReceiveDeathLink();
        void ConnectionTimerExpired();

        AP_ConnectionStatus connection_status;
        const std::chrono::seconds connection_timer(15);
        int slot_number;
        bool death_link_locked;
        const std::chrono::seconds death_link_timer(4);
    } // End private members


    void Client::Connect(const char* new_ip, const char* new_slot_name, const char* new_password) {
        // Initialize game state.
        GameData::Initialize();
        AP_Init(new_ip, "Pseudoregalia", new_slot_name, new_password);

        // Set all the necessary callbacks and info through APCpp.
        AP_NetworkVersion version{ 0, 6, 0 };
        AP_SetClientVersion(&version);
        AP_SetItemClearCallback(&GameData::Initialize); // Yes, this calls Initialize twice. I'll fix it when I add save files.
        AP_SetLocationCheckedCallback(&GameData::CheckLocation);
        AP_SetItemRecvCallback(&ReceiveItem);
        AP_SetDeathLinkSupported(true);
        AP_SetDeathLinkRecvCallback(&ReceiveDeathLink);
        AP_RegisterSlotDataIntCallback("slot_number", &SetSlotNumber);
        // TODO: Figure out a way to generalize this; might require lambdas?
        AP_RegisterSlotDataIntCallback("split_sun_greaves", &SetSplitKicks);

        AP_Start();

        // Print feedback to the player so they know the connect command went through.
        connection_status = AP_GetConnectionStatus();
        std::string connect_message = "Attempting to connect to ";
        connect_message.append(new_ip);
        connect_message += " with name ";
        connect_message.append(new_slot_name);
        Logger::Log(connect_message, Logger::LogType::System);
        Timer::RunTimerRealTime(connection_timer, &ConnectionTimerExpired);
    }

    void Client::SendCheck(int64_t id, std::wstring current_world) {
        Logger::Log(L"Sending check with id " + std::to_wstring(id));
        AP_SendItem(id);
    }
    
    // Sends game completion flag to Archipelago.
    void Client::CompleteGame() {
        AP_StoryComplete();

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
        if (AP_IsMessagePending()) {
            AP_Message* message = AP_GetLatestMessage();
            Logger::Log(message->text, Logger::LogType::Popup);
            AP_ClearLatestMessage();
            // APCpp releases the memory of message
        }

        if (ConnectionStatusChanged()) {
            if (connection_status == AP_ConnectionStatus::Authenticated) {
                Engine::SpawnCollectibles();
            }
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
        Timer::RunTimerRealTime(death_link_timer, &death_link_locked);
    }


    // Private functions
    namespace {
        void SetSlotNumber(int num) {
            slot_number = num;
        }

        void SetSplitKicks(int is_true) {
            GameData::SetOption("split_sun_greaves", is_true);
        }

        void ReceiveItem(int64_t id, bool notify) {
            Logger::Log(L"Receiving item with id " + std::to_wstring(id));
            GameData::ReceiveItem(id);
            Engine::SyncItems();
        }

        void ReceiveDeathLink() {
            if (death_link_locked) {
                return;
            }
            Logger::Log(L"Receiving death link");
            Engine::VaporizeGoat();
            Timer::RunTimerRealTime(death_link_timer, &death_link_locked);
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