#pragma once
#include "APClient.hpp"
#include <Mod/CppUserModBase.hpp>


namespace Pseudoregalia_AP {
    const char* ip;
    const char* slot_name;
    const char* password;

    APClient::APClient() {
        // probably take in slot name and ip here instead
    }

    void APClient::Connect(const char* new_ip, const char* new_slot_name, const char* new_password) {
        ip = new_ip;
        slot_name = new_slot_name;
        password = new_password;
        Initialize();
    }

    void APClient::Initialize() {
        AP_Init(ip, "Pseudoregalia", slot_name, password);

        // These are static but might fuck with members of APClient so... I might need to refactor this entire thing
        AP_SetItemClearCallback(&ClearItems);
        AP_SetItemRecvCallback(&ReceiveItem);
        AP_SetLocationCheckedCallback(&CheckLocation);

        AP_Start();
    }

    void APClient::ClearItems() {
    }

    void APClient::ReceiveItem(int64_t, bool) {

    }

    void APClient::CheckLocation(int64_t) {
    }

    void APClient::SendCheck(int id) {
        AP_SendItem(id);
    }
}