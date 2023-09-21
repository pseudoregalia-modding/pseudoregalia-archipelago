#pragma once
#include "APClient.hpp"

namespace Pseudoregalia_AP {
    const char* ip;
    const char* slot_name;
    const char* password;
    int APClient::connection_timer;
    AP_ConnectionStatus APClient::connection_status;

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
        {2365810011, L"extraKick"},
        {2365810012, L"airRecovery"},
        {2365810013, L"mobileHeal"},
        {2365810014, L"magicHaste"},
        {2365810015, L"healBoost"},
        {2365810016, L"damageBoost"},
        {2365810017, L"magicPiece"},
        {2365810018, L"outfitPro"},
    };

    std::map <std::wstring, std::vector<APCollectible>> APClient::zone_table;
    std::map<std::wstring, int> APClient::upgrade_table;
    bool APClient::major_keys[5];
    int APClient::small_keys;

    std::map<std::wstring, int> APClient::GetUpgradeTable() {
        return upgrade_table;
    }

    int APClient::GetSmallKeys() {
        return small_keys;
    }

    bool* APClient::GetMajorKeys() {
        return major_keys;
    }

    ItemType APClient::GetItemType(int64_t id) {
        id -= 2365810000;
        if (1 <= id && id <= 10) {
            return ItemType::Ability;
        }
        if (11 <= id && id <= 15) {
            return ItemType::MajorKey;
        }
        return ItemType::Unknown;
    }

    void APClient::FillZoneTable() {
        zone_table = {
            {L"ZONE_Dungeon", std::vector<APCollectible> {
                APCollectible("Dream Breaker", FVector(-3500.0, 4950.0, -50.0), 2365810001),
                APCollectible("Slide", FVector(16650, 2600, 2350), 2365810002),
                APCollectible("Dungeon Good Graces", FVector(18250, -9750, 4200), 2365810016),
                APCollectible("Strong Eyes Key", FVector(750, 8850, 2650), 2365810024),
                APCollectible("Rafters Key", FVector(7487, 1407, 4250), 2365810023)
                }},
            {L"ZONE_LowerCastle", std::vector<APCollectible> {
                APCollectible("Indignation", FVector(5400, 2100, -550), 2365810003),
                APCollectible("Professionalism", FVector(2700, -1700, -500), 2365810022),
                APCollectible("Castle Clear Mind", FVector(3390, 21150, 6600), 2365810019),
                APCollectible("Castle Good Graces", FVector(-3150, 11500, 6300), 2365810017),
                APCollectible("Pilgrimage", FVector(-5000, -600, 2050), 2365810013),
                APCollectible("Castle Hallway Key", FVector(7950, 2750, -200), 2365810025),
                APCollectible("Castle High Key", FVector(-4100, -8200, 2950), 2365810026)
                }},
            {L"Zone_Upper", std::vector<APCollectible>{
                APCollectible("Sunsetter", FVector(-3000, 4900, -400), 2365810004),
                APCollectible("Strikebreak", FVector(10050, 1800, 1000), 2365810005),
                APCollectible("Keep Major Key", FVector(14350, -50, 1350), 2365810049),
                APCollectible("Keep Clear Mind", FVector(-3900, -6109, -450), 2365810021),
                APCollectible("Keep Small Key", FVector(1050, 15700, 1300), 2365810027)
                }},
            {L"Zone_Library", std::vector<APCollectible>{
                APCollectible("Sun Greaves", FVector(-4150, 9200, -100), 2365810006),
                APCollectible("Clear Mind", FVector(-1300, -6750, -700), 2365810020)
                }},
            {L"Zone_Theatre", std::vector<APCollectible>{
                APCollectible("Soul Cutter", FVector(8500, 7850, -1400), 2365810007),
                APCollectible("Theatre Major Key", FVector(5200, 1550, 700), 2365810050),
                APCollectible("Aerial Finesse", FVector(-14100, -150, 1950), 2365810012),
                APCollectible("Theatre Empathy", FVector(-1460, -2550, 2240), 2365810015)
                }},
            {L"ZONE_Exterior", std::vector<APCollectible>{
                APCollectible("Solar Wind", FVector(-1100, 10850, 150), 2365810008),
                APCollectible("Bailey Major Key", FVector(-1787, 5236, 650), 2365810046),
                APCollectible("Bailey Empathy", FVector(5040, 7150, 2500), 2365810014),
                APCollectible("Bailey Small Key", FVector(3007, 3457, 300), 2365810028)
                }},
            {L"Zone_Caves", std::vector<APCollectible>{
                APCollectible("Ascendant Light", FVector(-5400, 6650, 6750), 2365810009),
                APCollectible("Underbelly Major Key", FVector(31900, 26250, 3850), 2365810047),
                APCollectible("Heliacal Power", FVector(18896, 7937, 1200), 2365810011),
                APCollectible("Martial Prowess", FVector(11300, 12700, 3107), 2365810018),
                APCollectible("Underbelly Key", FVector(-726, 19782, 3200), 2365810029)
                }},
            {L"Zone_Tower", std::vector<APCollectible>{
                APCollectible("Cling Gem", FVector(13350, 5250, 4150), 2365810010),
                APCollectible("Tower Major Key", FVector(9650, 5250, 7100), 2365810048)
                }},
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
            {L"extraKick", 0},
            {L"airRecovery", 0},
            {L"mobileHeal", 0},
            {L"magicHaste", 0},
            {L"healBoost", 0},
            {L"damageBoost", 0},
            {L"magicPiece", 0},
            {L"outfitPro", 0},
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
        connection_timer = 4000;
        connection_status = AP_GetConnectionStatus();
        std::string connect_message = "Attempting to connect to ";
        connect_message.append(new_ip);
        connect_message += " with name ";
        connect_message.append(new_slot_name);
        APGameManager::QueueMessage(connect_message);
    }

    void APClient::ClearItems() {
        Output::send<LogLevel::Verbose>(STR("Calling ClearItems...\n"));
        ResetUpgradeTable();
    }

    void APClient::ReceiveItem(int64_t new_item_id, bool notify) {
        // TODO: these id ranges should be defined with a constant
        if (GetItemType(new_item_id) == ItemType::Ability) {
            upgrade_table[lookup_id_to_item[new_item_id]]++;
            Output::send<LogLevel::Verbose>(STR("Set {} to {}\n"), lookup_id_to_item[new_item_id], upgrade_table[lookup_id_to_item[new_item_id]]);
        }
        else if (GetItemType(new_item_id) == ItemType::SmallKey) {
            small_keys++;
        }
        else if (GetItemType(new_item_id) == ItemType::MajorKey) {
            int key_index = new_item_id - 2365810011;
            major_keys[key_index] = true;
        }
        else {
            APGameManager::QueueMessage("Error: Unknown item ID received. Please check that you and the host are using the same version.");
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