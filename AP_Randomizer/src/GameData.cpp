#pragma once
#include "GameData.hpp"
#include "Logger.hpp"

namespace GameData {
    ItemType GetItemType(int64_t);

    int health_pieces;
    int small_keys;
    bool major_keys[5];
    std::map<std::wstring, int> upgrade_table;
    std::unordered_map<std::wstring, std::unordered_map<int64_t, Collectible>> collectible_table;

    const std::map<int64_t, std::wstring> lookup_id_to_upgrade = {
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

    int GameData::GetHealthPieces() {
        return health_pieces;
    }

    int GameData::GetSmallKeys() {
        return small_keys;
    }

    bool* GameData::GetMajorKeys() {
        return major_keys;
    }

    std::map<std::wstring, int> GameData::GetUpgradeTable() {
        return upgrade_table;
    }

    std::unordered_map<int64_t, Collectible> GameData::GetCollectiblesOfZone(std::wstring world_name) {
        return collectible_table[world_name];
    }

    void GameData::Initialize() {
        collectible_table = {
            {L"ZONE_Dungeon", std::unordered_map<int64_t, Collectible>{
            // Dream Breaker
                {2365810001, Collectible(FVector(-3500.0, 4950.0, -50.0))},
            // Slide
                {2365810002, Collectible(FVector(16650, 2600, 2350))},
            // Good Graces, dark rooms
                {2365810016, Collectible(FVector(18250, -9750, 4200))},
            // Small key, Strong Eyes
                {2365810024, Collectible(FVector(750, 8850, 2650))},
            // Small key, rafters
                {2365810023, Collectible(FVector(7487, 1407, 4250))},
            // HP, alcove by mirror
                {2365810030, Collectible(FVector(1150, -400, 1050))},
            // HP, past poles
                {2365810031, Collectible(FVector(6800, 8850, 3850))},
                    }},
            {L"ZONE_LowerCastle", std::unordered_map<int64_t, Collectible> {
            // Indignation
                {2365810003, Collectible(FVector(5400, 2100, -550))},
            // Locked door
                {2365810022, Collectible(FVector(2700, -1700, -500))},
            // Pilgrimage, floater in courtyard
                {2365810013, Collectible(FVector(-5000, -600, 2050))},
            // Small key, platform in main halls
                {2365810025, Collectible(FVector(7950, 2750, -200))},
            // Small key, high climb near wheel crawlers
                {2365810026, Collectible(FVector(-4100, -8200, 2950))},
            // Clear Mind, near theatre
                {2365810017, Collectible(FVector(3390, 21150, 6600))},
            // Good Graces, high climb from courtyard
                {2365810019, Collectible(FVector(-3150, 11500, 6300))},
            // HP, near dungeon
                {2365810032, Collectible(FVector(1600, 8000, -1400))},
            // HP, corner corridor
                {2365810033, Collectible(FVector(11850, 1000, -300))},
            // HP, wheel Crawlers
                {2365810034, Collectible(FVector(-10050, -3700, 1000))},
            // HP, alcove near theatre
                {2365810035, Collectible(FVector(-9600, 21750, 5400))},
            // HP, balcony
                {2365810036, Collectible(FVector(16400, 3800, 1200))},
                    }},
            {L"Zone_Upper", std::unordered_map<int64_t, Collectible> {
            // Sunsetter
                {2365810004, Collectible(FVector(-3000, 4900, -400))},
            // Strikebreak
                {2365810005, Collectible(FVector(10050, 1800, 1000))},
            // Keep major key
                {2365810049, Collectible(FVector(14350, -50, 1350))},
            // Clear Mind near theatre
                {2365810021, Collectible(FVector(-3900, -6109, -450))},
            // small key, lever room
                {2365810027, Collectible(FVector(1050, 15700, 1300))},
                // HP, alcove near locked door
                {2365810039, Collectible(FVector(800, 2500, 1200))},
                    }},
            {L"Zone_Library", std::unordered_map<int64_t, Collectible> {
            // sun greaves
                {2365810006, Collectible(FVector(-4150, 9200, -100))},
            // Clear mind, locked left
                {2365810020, Collectible(FVector(-1300, -6750, -700))},
            // HP upper
                {2365810037, Collectible(FVector(-9250, -1850, 1250))},
            // HP locked
                {2365810038, Collectible(FVector(-3750, -4170, -700))},
                    }},
            {L"Zone_Theatre", std::unordered_map<int64_t, Collectible> {
            // soul cutter
                {2365810007, Collectible(FVector(8500, 7850, -1400))},
            // theatre major key
                {2365810050, Collectible(FVector(5200, 1550, 700))},
            // aerial finesse on ceiling beam
                {2365810012, Collectible(FVector(-14100, -150, 1950))},
            // empathy, locked door
                {2365810015, Collectible(FVector(-1460, -2550, 2240))},
            // HP, murder goat
                {2365810044, Collectible(FVector(255, 1150, 50))},
            // HP, back of auditorium
                {2365810045, Collectible(FVector(-1600, 1500, 2600))},
                    }},
            {L"ZONE_Exterior", std::unordered_map<int64_t, Collectible> {
            // solar wind
                {2365810008, Collectible(FVector(-1100, 10850, 150))},
            // bailey major key
                {2365810046, Collectible(FVector(-1787, 5236, 650))},
            // empathy, cheese bell
                {2365810014, Collectible(FVector(5040, 7150, 2500))},
            // small key, in a bulding
                {2365810028, Collectible(FVector(3007, 3457, 300))},
            // hp, center steeple
                {2365810040, Collectible(FVector(2350, 7260, 2110))},
                    }},
            {L"Zone_Caves", std::unordered_map<int64_t, Collectible> {
            // ascendant light
                {2365810009, Collectible(FVector(-5400, 6650, 6750))},
            // underbelly major key
                {2365810047, Collectible(FVector(31900, 26250, 3850))},
            // heliacal power, locked door
                {2365810011, Collectible(FVector(18896, 7937, 1200))},
            // martial prowess, strikebreak wall
                {2365810018, Collectible(FVector(11300, 12700, 3107))},
            // small key, main room
                {2365810029, Collectible(FVector(-726, 19782, 3200))},
            // HP, rafters near keep
                {2365810041, Collectible(FVector(19600, 17750, 5700))},
            // HP, building near little guy
                {2365810042, Collectible(FVector(-4350, 28350, 1850))},
            // HP, alcove in light path
                {2365810043, Collectible(FVector(-2550, 12300, 4400))},
                    }},
            {L"Zone_Tower", std::unordered_map<int64_t, Collectible> {
            // cling gem
                {2365810010, Collectible(FVector(13350, 5250, 4150))},
            // tower major key
                {2365810048, Collectible(FVector(9650, 5250, 7100))},
                    }},
        };

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

    ItemType GameData::GetItemType(int64_t id) {
        id -= 2365810000;
        if (1 <= id && id <= 18) {
            return ItemType::Ability;
        }
        if (id == 19) {
            return ItemType::HealthPiece;
        }
        if (id == 20) {
            return ItemType::SmallKey;
        }
        if (21 <= id && id <= 25) {
            return ItemType::MajorKey;
        }
        return ItemType::Unknown;
    }

    ItemType GameData::ReceiveItem(int64_t id) {
        ItemType type = GetItemType(id);
        switch (type) {
        case ItemType::Ability:
            upgrade_table[lookup_id_to_upgrade.at(id)]++;
            break;
        case ItemType::HealthPiece:
            health_pieces++;
            break;
        case ItemType::SmallKey:
            small_keys++;
            break;
        case ItemType::MajorKey:
            // Remove prefix digits from id to assign directly to major_keys array
            major_keys[id - 2365810021] = true;
            break;
        default:
            Logger::Log(L"You were sent an item, but its id wasn't recognized. Verify that you're playing on the same version this seed was generated on.");
            break;
        }
        return type;
    }

    void GameData::CheckLocation(int64_t id) {
        // Iterate through each outer map and check if the inner map has the input id.
        for (auto& zone : collectible_table) {
            std::unordered_map<int64_t, Collectible>::iterator iter = zone.second.find(id);
            if (iter != zone.second.end()) {
                iter->second.Check();
                return;
            }
        }

        Logger::Log(L"No location with id " + std::to_wstring(id) + L" was found. The developer probably made a mistake in the internal data.", Logger::LogType::Error);
    }
}