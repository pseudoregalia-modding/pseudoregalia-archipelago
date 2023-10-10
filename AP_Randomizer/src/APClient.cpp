#pragma once
#include "APClient.hpp"

namespace Pseudoregalia_AP {
    const char* ip;
    const char* slot_name;
    const char* password;
    int APClient::slot_number;
    int APClient::connection_timer;
    AP_ConnectionStatus APClient::connection_status;

    void APClient::Connect(const char* new_ip, const char* new_slot_name, const char* new_password) {
        AP_Init(new_ip, "Pseudoregalia", new_slot_name, new_password);
        AP_SetItemClearCallback(&ClearItems);
        AP_SetItemRecvCallback(&ReceiveItem);
        AP_SetLocationCheckedCallback(&CheckLocation);
        AP_RegisterSlotDataIntCallback("slot_number", &SetSlotNumber);
        AP_Start();
        connection_timer = 4000;
        connection_status = AP_GetConnectionStatus();
        std::string connect_message = "Attempting to connect to ";
        connect_message.append(new_ip);
        connect_message += " with name ";
        connect_message.append(new_slot_name);
        APGameManager::QueueMessage(connect_message);
    }

    void APClient::SetSlotNumber(int num) {
        slot_number = num;
    }

    void APClient::ClearItems() {
        Output::send<LogLevel::Verbose>(STR("Calling ClearItems...\n"));
        ResetUpgradeTable();
    }

    void APClient::ReceiveItem(int64_t id, bool notify) {
        GameData::ReceiveItem(id);
        Engine::SyncItems();
    }

    void APClient::SendCheck(int64_t id, std::wstring current_world) {
        for (APCollectible &collectible : zone_table[current_world]) {
            if (collectible.GetID() == id) {
                AP_SendItem(id);
                // Archipelago will call CheckLocation itself
                return;
            }
        }
    }

    void APClient::CheckLocation(int64_t id) {
        for (auto& zone_data : zone_table) {
            std::vector<APCollectible> collectible_vector = zone_data.second;
            for (APCollectible& collectible : zone_data.second) {
                if (collectible.GetID() == id) {
                    collectible.Check();
                    return;
                }
            }
        }
        Output::send<LogLevel::Error>(STR("APClient::CheckLocation: No location with id {} could be found."), id);
    }
    
    void APClient::CompleteGame() {
        AP_StoryComplete();

        // Send a key to datastorage upon game completion for PopTracker integration
        AP_SetServerDataRequest* completion_flag = new AP_SetServerDataRequest();
        AP_DataStorageOperation operation = *new AP_DataStorageOperation();
        int filler_value = 0;
        operation.operation = "add";
        operation.value = &filler_value;
        completion_flag->key = "Pseudoregalia - Player " + std::to_string(slot_number) + " - Game Complete";
        completion_flag->type = AP_DataType::Int;
        completion_flag->want_reply = true;
        completion_flag->operations.push_back(operation);
        AP_SetServerData(completion_flag);
    }

    bool APClient::ConnectionStatusChanged() {
        if (connection_status != AP_GetConnectionStatus()) {
            connection_status = AP_GetConnectionStatus();
            return true;
        }
        return false;
    }

    void APClient::PollServer() {
        if (ConnectionStatusChanged()) {
            if (connection_status == AP_ConnectionStatus::Authenticated) {
                APGameManager::SetClientConnected(true);
                connection_timer = 0;
            }
            if (connection_status == AP_ConnectionStatus::ConnectionRefused) {
                APGameManager::QueueMessage("The server refused the connection. Please double-check your connection info and restart the game.");
                connection_timer = 0;
            }
        }

        if (connection_timer > 0) {
            connection_timer--;
            if (connection_timer <= 0) {
                APGameManager::QueueMessage("Could not find the address entered. Please double-check your connection info and restart the game.");
            }
        }

        if (AP_IsMessagePending()) {
            AP_Message* message = AP_GetLatestMessage();
            APGameManager::QueueMessage(message->text);
            printf(message->text.c_str());
            printf("\n");
            AP_ClearLatestMessage();
        }
    }
}