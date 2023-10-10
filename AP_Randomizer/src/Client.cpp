#pragma once
#include "Client.hpp"

namespace Client {
    void ClearItems();
    void ReceiveItem(int64_t, bool);
    void CheckLocation(int64_t);
    bool ConnectionStatusChanged();
    void SetSlotNumber(int);
    int connection_timer;
    AP_ConnectionStatus connection_status;
    int slot_number;

    void Client::Connect(const char* new_ip, const char* new_slot_name, const char* new_password) {
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
        Logger::PrintToPlayer(connect_message);
    }

    void Client::SetSlotNumber(int num) {
        slot_number = num;
    }

    void Client::ClearItems() {
        GameData::Initialize();
    }

    void Client::ReceiveItem(int64_t id, bool notify) {
        GameData::ReceiveItem(id);
        Engine::SyncItems();
    }

    void Client::SendCheck(int64_t id, std::wstring current_world) {
        std::vector<GameData::Collectible> collectible_vector = GameData::GetCollectiblesOfZone(Engine::GetWorld()->GetName());
        for (GameData::Collectible& collectible : collectible_vector) {
            if (collectible.GetID() == id) {
                AP_SendItem(id);
                return;
            }
        }
    }

    void Client::CheckLocation(int64_t id) {
        GameData::CheckLocation(id);
    }
    
    void Client::CompleteGame() {
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

    bool Client::ConnectionStatusChanged() {
        if (connection_status != AP_GetConnectionStatus()) {
            connection_status = AP_GetConnectionStatus();
            return true;
        }
        return false;
    }

    void Client::PollServer() {
        if (ConnectionStatusChanged()) {
            if (connection_status == AP_ConnectionStatus::Authenticated) {
                GameManager::SetClientConnected(true);
                connection_timer = 0;
            }
            if (connection_status == AP_ConnectionStatus::ConnectionRefused) {
                Logger::PrintToPlayer("The server refused the connection. Please double-check your connection info and restart the game.");
                connection_timer = 0;
            }
        }

        if (connection_timer > 0) {
            connection_timer--;
            if (connection_timer <= 0) {
                Logger::PrintToPlayer("Could not find the address entered. Please double-check your connection info and restart the game.");
            }
        }

        if (AP_IsMessagePending()) {
            AP_Message* message = AP_GetLatestMessage();
            Logger::PrintToPlayer(message->text);
            printf(message->text.c_str());
            printf("\n");
            AP_ClearLatestMessage();
        }
    }
}