#pragma once
#include "APClient.hpp"
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>

using namespace RC;
using namespace RC::Unreal;

namespace Pseudoregalia_AP {
    const char* ip;
    const char* slot_name;
    const char* password;

    struct CollectibleSpawnInfo {
        int64_t id;
        FVector position;
        bool valid;

        void FillSpawnInfo(int64_t, FVector);
    };

    void CollectibleSpawnInfo::FillSpawnInfo(int64_t new_id, FVector new_position) {
        id = new_id;
        position = new_position;
    }

    std::map<std::string, int> obtained_upgrades;

    std::map <std::string, std::vector<APCollectible>> zone_table;
    APClient::APClient() {
        FillTable();
    }

    void APClient::FillTable() {
        this->zone_table = {
            { "ZONE_Dungeon", std::vector<APCollectible>{APCollectible("Dungeon", FVector(-3500.0, 4950.0, -50.0), 2365810001), APCollectible("Dungeon", FVector(16650, 2600, 2350), 2365810002)}},
            { "ZONE_LowerCastle", std::vector<APCollectible>{APCollectible("Castle", FVector(5400, 2100, -550), 2365810003)} },
            { "Zone_Upper", std::vector<APCollectible>{APCollectible("Dungeon", FVector(-3000, 4900, -400), 2365810004), APCollectible("Dungeon", FVector(10050, 1800, 85), 2365810005)} },
            { "Zone_Library", std::vector<APCollectible>{APCollectible("Library", FVector(-4150, 9200, -100), 2365810006)} },
            { "Zone_Theatre", std::vector<APCollectible>{APCollectible("Theatre", FVector(8500, 7850, -1400), 2365810007)} },
            { "ZONE_Exterior", std::vector<APCollectible>{APCollectible("Bailey", FVector(-1100, 10850, 150), 2365810008)} },
            { "Zone_Caves", std::vector<APCollectible>{APCollectible("Underbelly", FVector(-5400, 6650, 6750), 2365810009)} },
            { "Zone_Tower", std::vector<APCollectible>{APCollectible("Tower", FVector(13350, 5250, 4150), 2365810010)} },
        };
    }

    void ClearItems() {
    }

    void ReceiveItem(int64_t, bool) {
    }

    void CheckLocation(int64_t) {
    }

    void SendCheck(int64_t id) {
        AP_SendItem(id);
    }

    void APClient::Connect(const char* new_ip, const char* new_slot_name, const char* new_password) {
        ip = new_ip;
        slot_name = new_slot_name;
        password = new_password;
        Initialize();
    }

    void APClient::Initialize() {
        AP_Init(ip, "Pseudoregalia", slot_name, password);

        AP_SetItemClearCallback(&ClearItems);
        AP_SetItemRecvCallback(&ReceiveItem);
        AP_SetLocationCheckedCallback(&CheckLocation);

        AP_Start();
    }

    void APClient::OnMapLoad(AActor* randomizer_blueprint, UFunction* spawn_function) {
        // Pretend it's Dungeon for now, will get actual map name later and probably switch with a function

        for (int i = 0; i < this->zone_table["Dungeon"].size(); i++) {
            Output::send<LogLevel::Verbose>(STR("id: {}"), this->zone_table["Dungeon"][i].id);

            CollectibleSpawnInfo new_info = {
                this->zone_table["Dungeon"][i].id,
                this->zone_table["Dungeon"][i].position,
                false
            };

            randomizer_blueprint->ProcessEvent(spawn_function, &new_info);
        }
    }
}