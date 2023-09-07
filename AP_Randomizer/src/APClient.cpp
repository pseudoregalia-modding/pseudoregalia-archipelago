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
        zone_table = {
            {"Dungeon", std::vector<APCollectible>{APCollectible("Dungeon", FVector(3500, 4950, -50), 2365810001), APCollectible("Dungeon", FVector(16650, 2600, 2350), 2365810002)}},
            { "Castle", std::vector<APCollectible>{APCollectible("Castle", FVector(5400, 2100, -550), 2365810003)} },
            { "Keep", std::vector<APCollectible>{APCollectible("Dungeon", FVector(-3000, 4900, -400), 2365810004), APCollectible("Dungeon", FVector(10050, 1800, 85), 2365810005)} },
            { "Theatre", std::vector<APCollectible>{APCollectible("Theatre", FVector(8500, 7850, -1400), 2365810005)} },
            { "Underbelly", std::vector<APCollectible>{APCollectible("Underbelly", FVector(-5400, 6650, 6750), 2365810006)} },
            { "Bailey", std::vector<APCollectible>{APCollectible("Bailey", FVector(-1100, 10850, 150), 2365810007)} },
            { "Tower", std::vector<APCollectible>{APCollectible("Tower", FVector(13350, 5250, 4150), 2365810008)} },
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

        for (int i = 0; i < zone_table["Dungeon"].size(); i++) {

            Output::send<LogLevel::Verbose>(STR("id: {}"), zone_table["Dungeon"][i].id);

            // Something is horribly wrong with the coordinates.
            // I can spawn them in the correct locations with hardcoded values,
            // but fetching them from zone_data results in ridiculously large X values with Y and Z at zero.
            // There seems to be some kind of underflow or bad conversion happening, but I have no idea what's causing it.
            // Maybe it could have to do with trying to pass implied ints into the blueprint??
            // 
            // (X=2064845619742928566623404032.000000,Y=0.000000,Z=0.000000)
            // (X=11182566371757979795980288.000000,Y=0.000000,Z=0.000000)
            CollectibleSpawnInfo new_info = {
                zone_table["Dungeon"][i].id,
                //FVector(16650, 2600, 2350),
                zone_table["Dungeon"][i].position,
                false
            };

            Output::send<LogLevel::Verbose>(STR("Two"));
            randomizer_blueprint->ProcessEvent(spawn_function, &new_info);
            Output::send<LogLevel::Verbose>(STR("Three"));
        }
    }
}