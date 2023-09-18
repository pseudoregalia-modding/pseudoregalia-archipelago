#pragma once
#include "APClient.hpp"

namespace Pseudoregalia_AP {
    const char* ip;
    const char* slot_name;
    const char* password;

    std::map<int64_t, std::wstring> APClient::lookup_id_to_item = {
        {2365810001, L"attack"},
        {2365810002, L"powerBoost"},
        {2365810003, L"airKick"},
        {2365810004, L"slide"},
        {2365810005, L"SlideJump"},
        {2365810006, L"plunge"},
        {2365810007, L"chargeAttack"},
        {2365810008, L"wallRide"},
        {2365810009, L"Light"},
        {2365810010, L"projectile"},
    };

    std::map <std::wstring, std::vector<APCollectible>> APClient::zone_table;
    std::map<std::wstring, int> APClient::upgrade_table;
    bool APClient::major_keys[5];

    std::map<std::wstring, int> APClient::GetUpgradeTable() {
        return upgrade_table;
    }

    bool* APClient::GetMajorKeys() {
        return major_keys;
    }

    void APClient::FillZoneTable() {
        zone_table = {
            {L"ZONE_Dungeon", std::vector<APCollectible> {
                APCollectible("Dungeon", FVector(-3500.0, 4950.0, -50.0), 2365810001),
                APCollectible("Dungeon", FVector(16650, 2600, 2350), 2365810002)}
                },
            {L"ZONE_LowerCastle", std::vector<APCollectible> {
                APCollectible("Castle", FVector(5400, 2100, -550), 2365810003)}
                },
            {L"Zone_Upper", std::vector<APCollectible>{
                APCollectible("Keep", FVector(-3000, 4900, -400), 2365810004),
                APCollectible("Keep", FVector(10050, 1800, 1000), 2365810005),
                APCollectible("Keep", FVector(14350, -50, 1350), 2365810014)}
                },
            {L"Zone_Library", std::vector<APCollectible>{
                APCollectible("Library", FVector(-4150, 9200, -100), 2365810006)}
                },
            {L"Zone_Theatre", std::vector<APCollectible>{
                APCollectible("Theatre", FVector(8500, 7850, -1400), 2365810007),
                APCollectible("Theatre", FVector(5200, 1550, 700), 2365810015)}
                },
            {L"ZONE_Exterior", std::vector<APCollectible>{
                APCollectible("Bailey", FVector(-1100, 10850, 150), 2365810008),
                APCollectible("Bailey", FVector(-1787, 5236, 650), 2365810011)}
                },
            {L"Zone_Caves", std::vector<APCollectible>{
                APCollectible("Underbelly", FVector(-5400, 6650, 6750), 2365810009),
                APCollectible("Underbelly", FVector(31900, 26250, 3850), 2365810012)
                }},
            {L"Zone_Tower", std::vector<APCollectible>{
                APCollectible("Tower", FVector(13350, 5250, 4150), 2365810010),
                APCollectible("Tower", FVector(9650, 5250, 7100), 2365810013)}
                },
        };
    }

    void APClient::ResetUpgradeTable() {
        upgrade_table = {
            {L"attack", 0},
            {L"powerBoost", 0},
            {L"airKick", 0},
            {L"slide", 0},
            {L"SlideJump", 0},
            {L"plunge", 0},
            {L"chargeAttack", 0},
            {L"wallRide", 0},
            {L"Light", 0},
            {L"projectile", 0},
        };
    }

    std::vector<APCollectible> APClient::GetCurrentZoneCollectibles(std::wstring world_name) {
        return zone_table[world_name];
    }

    void APClient::Initialize() {
        FillZoneTable();
        ResetUpgradeTable();
    }

    void APClient::Connect(const char* new_ip, const char* new_slot_name, const char* new_password) {
        AP_Init(new_ip, "Pseudoregalia", new_slot_name, new_password);
        AP_SetItemClearCallback(&ClearItems);
        AP_SetItemRecvCallback(&ReceiveItem);
        AP_SetLocationCheckedCallback(&CheckLocation);
        AP_Start();
    }

    void APClient::ClearItems() {
        Output::send<LogLevel::Verbose>(STR("Calling ClearItems...\n"));
        ResetUpgradeTable();
    }

    void APClient::ReceiveItem(int64_t new_item_id, bool notify) {
        // TODO: these id ranges should be defined with a constant
        if (2365810011 <= new_item_id && new_item_id <= 2365810015) {
            int key_index = new_item_id - 2365810011;
            major_keys[key_index] = true;
        }
        else {
            upgrade_table[lookup_id_to_item[new_item_id]]++;
            Output::send<LogLevel::Verbose>(STR("Set {} to {}\n"), lookup_id_to_item[new_item_id], upgrade_table[lookup_id_to_item[new_item_id]]);
        }

        APGameManager::QueueItemUpdate();
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
        std::wstring current_world = APGameManager::GetWorld()->GetName();
        for (APCollectible& collectible : zone_table[current_world]) {
            if (collectible.GetID() == id) {
                collectible.Check();
            }
        }
    }

    void APClient::PollServer() {
        if (AP_IsInit()) {
            printf("init'ed!");
        }
    }
}