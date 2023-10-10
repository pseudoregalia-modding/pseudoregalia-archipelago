#pragma once
#include "GameData.hpp"

namespace Pseudoregalia_AP {
    const std::map<int64_t, std::wstring> GameData::lookup_id_to_upgrade = {
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

    void GameData::Initialize() {
        collectible_table = {
            {L"ZONE_Dungeon", std::vector<APCollectible> {
                APCollectible("Dream Breaker", FVector(-3500.0, 4950.0, -50.0), 2365810001),
                APCollectible("Slide", FVector(16650, 2600, 2350), 2365810002),
                APCollectible("Good Graces", FVector(18250, -9750, 4200), 2365810016),
                APCollectible("Strong Eyes", FVector(750, 8850, 2650), 2365810024),
                APCollectible("Rafters Key", FVector(7487, 1407, 4250), 2365810023),
                APCollectible("HP Alcove", FVector(1150, -400, 1050), 2365810030),
                APCollectible("HP Poles", FVector(6800, 8850, 3850), 2365810031)
                }},
            {L"ZONE_LowerCastle", std::vector<APCollectible> {
                APCollectible("Indignation", FVector(5400, 2100, -550), 2365810003),
                APCollectible("Professionalism", FVector(2700, -1700, -500), 2365810022),
                APCollectible("Pilgrimage", FVector(-5000, -600, 2050), 2365810013),
                APCollectible("Hall Key", FVector(7950, 2750, -200), 2365810025),
                APCollectible("High Key", FVector(-4100, -8200, 2950), 2365810026),
                APCollectible("Clear Mind", FVector(3390, 21150, 6600), 2365810017),
                APCollectible("Good Graces", FVector(-3150, 11500, 6300), 2365810019),
                APCollectible("HP Dungeon", FVector(1600, 8000, -1400), 2365810032),
                APCollectible("HP Corridor", FVector(11850, 1000, -300), 2365810033),
                APCollectible("HP Wheels", FVector(-10050, -3700, 1000), 2365810034),
                APCollectible("HP Scythes", FVector(-9600, 21750, 5400), 2365810035),
                APCollectible("HP Balcony", FVector(16400, 3800, 1200), 2365810036)
                }},
            {L"Zone_Upper", std::vector<APCollectible>{
                APCollectible("Sunsetter", FVector(-3000, 4900, -400), 2365810004),
                APCollectible("Strikebreak", FVector(10050, 1800, 1000), 2365810005),
                APCollectible("Keep Major", FVector(14350, -50, 1350), 2365810049),
                APCollectible("Clear Mind", FVector(-3900, -6109, -450), 2365810021),
                APCollectible("Keep Key", FVector(1050, 15700, 1300), 2365810027),
                APCollectible("HP Alcove", FVector(800, 2500, 1200), 2365810039)
                }},
            {L"Zone_Library", std::vector<APCollectible>{
                APCollectible("Sun Greaves", FVector(-4150, 9200, -100), 2365810006),
                APCollectible("Clear Mind", FVector(-1300, -6750, -700), 2365810020),
                APCollectible("HP Upper", FVector(-9250, -1850, 1250), 2365810037),
                APCollectible("HP Locked", FVector(-3750, -4170, -700), 2365810038)
                }},
            {L"Zone_Theatre", std::vector<APCollectible>{
                APCollectible("Soul Cutter", FVector(8500, 7850, -1400), 2365810007),
                APCollectible("Theatre Major", FVector(5200, 1550, 700), 2365810050),
                APCollectible("Aerial Finesse", FVector(-14100, -150, 1950), 2365810012),
                APCollectible("Theatre Empathy", FVector(-1460, -2550, 2240), 2365810015),
                APCollectible("HP Murder Goat", FVector(255, 1150, 50), 2365810044),
                APCollectible("HP Auditorium", FVector(-1600, 1500, 2600), 2365810045)
                }},
            {L"ZONE_Exterior", std::vector<APCollectible>{
                APCollectible("Solar Wind", FVector(-1100, 10850, 150), 2365810008),
                APCollectible("Bailey Major", FVector(-1787, 5236, 650), 2365810046),
                APCollectible("Bailey Empathy", FVector(5040, 7150, 2500), 2365810014),
                APCollectible("Small Key", FVector(3007, 3457, 300), 2365810028),
                APCollectible("HP Steeple", FVector(2350, 7260, 2110), 2365810040)
                }},
            {L"Zone_Caves", std::vector<APCollectible>{
                APCollectible("Ascendant Light", FVector(-5400, 6650, 6750), 2365810009),
                APCollectible("UnderbellyMajor", FVector(31900, 26250, 3850), 2365810047),
                APCollectible("Heliacal Power", FVector(18896, 7937, 1200), 2365810011),
                APCollectible("Martial Prowess", FVector(11300, 12700, 3107), 2365810018),
                APCollectible("Underbelly Key", FVector(-726, 19782, 3200), 2365810029),
                APCollectible("HP Rafters", FVector(19600, 17750, 5700), 2365810041),
                APCollectible("HP Little Guy", FVector(-4350, 28350, 1850), 2365810042),
                APCollectible("HP Alcove", FVector(-2550, 12300, 4400), 2365810043)
                }},
            {L"Zone_Tower", std::vector<APCollectible>{
                APCollectible("Cling Gem", FVector(13350, 5250, 4150), 2365810010),
                APCollectible("Tower Major Key", FVector(9650, 5250, 7100), 2365810048)
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

    void GameData::ReceiveItem(int64_t id) {
        switch (GetItemType(id)) {
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
            // return an error
            break;
        }
    }
}