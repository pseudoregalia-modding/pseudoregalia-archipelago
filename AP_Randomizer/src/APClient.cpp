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
    bool item_update_pending;

    struct CollectibleSpawnInfo {
        int64_t id;
        FVector position;
        bool valid;

        void FillSpawnInfo(int64_t, FVector);
    };

    struct AddUpgradeInfo {
        FName name;
        int count;
    };

    void CollectibleSpawnInfo::FillSpawnInfo(int64_t new_id, FVector new_position) {
        id = new_id;
        position = new_position;
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
                APCollectible("Keep", FVector(10050, 1800, 85), 2365810005)}
                },
            {L"Zone_Library", std::vector<APCollectible>{
                APCollectible("Library", FVector(-4150, 9200, -100), 2365810006)}
                },
            {L"Zone_Theatre", std::vector<APCollectible>{
                APCollectible("Theatre", FVector(8500, 7850, -1400), 2365810007)}
                },
            {L"ZONE_Exterior", std::vector<APCollectible>{
                APCollectible("Bailey", FVector(-1100, 10850, 150), 2365810008)}
                },
            {L"Zone_Caves", std::vector<APCollectible>{
                APCollectible("Underbelly", FVector(-5400, 6650, 6750), 2365810009)
                }},
            {L"Zone_Tower", std::vector<APCollectible>{
                APCollectible("Tower", FVector(13350, 5250, 4150), 2365810010)}
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

    void APClient::Initialize() {
        FillZoneTable();
        ResetUpgradeTable();
        item_update_pending = false;
    }

    void APClient::Connect(const char* new_ip, const char* new_slot_name, const char* new_password) {
        AP_Init(new_ip, "Pseudoregalia", new_slot_name, new_password);
        AP_SetItemClearCallback(&ClearItems);
        AP_SetItemRecvCallback(&ReceiveItem);
        AP_SetLocationCheckedCallback(&CheckLocation);
        AP_Start();
    }

    void APClient::ClearItems() {
        Output::send<LogLevel::Verbose>(STR("Calling ClearItems..."));
        ResetUpgradeTable();
    }

    void APClient::ReceiveItem(int64_t new_item_id, bool notify) {
        upgrade_table[lookup_id_to_item[new_item_id]] ++;
        Output::send<LogLevel::Verbose>(STR("Set {} to {}"), lookup_id_to_item[new_item_id], upgrade_table[lookup_id_to_item[new_item_id]]);
        item_update_pending = true;
    }

    void APClient::CheckLocation(int64_t) {
    }

    void APClient::SendCheck(int64_t id, std::wstring current_world) {
        for (APCollectible &collectible : zone_table[current_world]) {
            if (collectible.GetID() == id) {
                collectible.Check();
                AP_SendItem(id);
                return;
            }
        }
    }

    void APClient::SyncItems() {
        UObject* randomizer_blueprint = UObjectGlobals::FindFirstOf(STR("BP_APRandomizerInstance_C"));
        if (!randomizer_blueprint) {
            // TODO: Make this set a timer to resync instead of just returning
            Output::send<LogLevel::Error>(STR("BP_APRandomizerInstance was not found. Items could not be synced."));
            return;
        }

        UFunction* add_upgrade_function = randomizer_blueprint->GetFunctionByName(STR("AP_AddUpgrade"));
        if (!add_upgrade_function) {
            // TODO: Make this set a timer to resync instead of just returning
            Output::send<LogLevel::Error>(STR("BP_APRandomizerInstance was found, but had no function AP_AddUpgrade. Items could not be synced."));
            return;
        }
        Output::send<LogLevel::Verbose>(STR("Calling SyncItems..."));

        for (auto const& pair : upgrade_table)
        {
            FName* name_ptr = new FName(pair.first);
            FName new_name = *name_ptr;

            int new_count = pair.second;

            AddUpgradeInfo params = {
                new_name,
                new_count,
            };
            Output::send<LogLevel::Verbose>(STR("Attempting to add {} with value {}..."), pair.first, new_count);

            randomizer_blueprint->ProcessEvent(add_upgrade_function, &params);
        }
    }

    void APClient::OnMapLoad(AActor* randomizer_blueprint, UFunction* spawn_function, std::wstring world_name) {
        if (!zone_table.count(world_name)) {
            return;
        }

        for (APCollectible collectible : zone_table[world_name]) {
            if (collectible.IsChecked()) {
                Output::send<LogLevel::Warning>(STR("Collectible with ID {} has already been sent"), collectible.GetID());
                continue;
            }
            Output::send<LogLevel::Verbose>(STR("Spawned collectible with ID {}"), collectible.GetID());

            CollectibleSpawnInfo new_info = {
                collectible.GetID(),
                collectible.GetPosition(),
                false
            };

            randomizer_blueprint->ProcessEvent(spawn_function, &new_info);
        }
    }
}