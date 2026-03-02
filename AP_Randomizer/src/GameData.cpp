#pragma once
#include "GameData.hpp"
#include "Logger.hpp"
#include "Settings.hpp"
#include "Client.hpp"
#include "Engine.hpp"

namespace GameData {
    using std::unordered_map;
    using std::list;
    using std::wstring;
    using std::string;
    using std::optional;
    using std::pair;
    using std::tuple;
    using std::vector;

    // Private members
    namespace {
        ItemType GetItemType(int64_t);
        optional<Interactable> GetInteractable(wstring);

        optional<wstring> note_being_read = {};

        int health_pieces;
        int small_keys;
        bool major_keys[5];
        vector<MultiworldLocation> major_key_hints[5];
        unordered_map<wstring, int> upgrade_table;
        unordered_map<Map, unordered_map<int64_t, Collectible>> collectible_table;
        unordered_map<Map, unordered_map<wstring, TimeTrial>> time_trial_table;
        unordered_map<int64_t, Classification> lookup_location_id_to_classification;
        unordered_map<string, int> options;
        bool slidejump_owned;
        bool slidejump_disabled;

        const vector<PlayerStart> player_starts = {
            { L"ZONE_Dungeon", L"gameStart", L"Dungeon Mirror" },
            { L"ZONE_Dungeon", L"dungeonlowestSave", L"Dungeon Mirror Save" },
            { L"ZONE_Dungeon", L"dungeonWestSave", L"Dungeon Slide Save" },
            { L"ZONE_Dungeon", L"dungeonSaveNearBoss", L"Dungeon Strong Eyes Save" },
            { L"ZONE_Dungeon", L"lower1", L"Dungeon Strong Eyes Exit" },
            { L"ZONE_Dungeon", L"dungeonWest", L"Dungeon Escape Lower Exit" },
            { L"ZONE_Dungeon", L"dungeonNorth", L"Dungeon Escape Upper Exit" },

            { L"ZONE_LowerCastle", L"lowerWestSave", L"Castle West Save" },
            { L"ZONE_LowerCastle", L"startGazebo", L"Castle Gazebo Save" },
            { L"ZONE_LowerCastle", L"lowerEastSave", L"Castle East Save" },
            { L"ZONE_LowerCastle", L"lowerNorthWestTheatre", L"Castle Northwest Save" },
            { L"ZONE_LowerCastle", L"dungeon1", L"Castle West Lower Exit" },
            { L"ZONE_LowerCastle", L"lowerWest", L"Castle West Upper Exit" },
            { L"ZONE_LowerCastle", L"exterior1", L"Castle South Lower Exit" },
            { L"ZONE_LowerCastle", L"lowerSouthHigh", L"Castle South Upper Exit" },
            { L"ZONE_LowerCastle", L"lowerMiddle", L"Castle Locked Exit" },
            { L"ZONE_LowerCastle", L"lowerEast", L"Castle East Exit" },
            { L"ZONE_LowerCastle", L"lowerNorth", L"Castle North Exit" },
            { L"ZONE_LowerCastle", L"lowerNorthNorthWest", L"Castle Northwest Exit" },

            { L"Zone_Upper", L"saveUpperMid", L"Keep Central Save" },
            { L"Zone_Upper", L"upperNorthSave", L"Keep North Save" },
            { L"Zone_Upper", L"upperSouth", L"Keep South Exit" },
            { L"Zone_Upper", L"upperSouthWest", L"Keep Southwest Exit" },
            { L"Zone_Upper", L"upperMiddle", L"Keep Locked Exit" },
            { L"Zone_Upper", L"upperNorthEast", L"Keep Northeast Exit" },
            { L"Zone_Upper", L"upperNorth", L"Keep North Exit" },

            { L"Zone_Library", L"librarySave", L"Library Main Save" },
            { L"Zone_Library", L"saveLibraryWest", L"Library Back Save" },
            { L"Zone_Library", L"libraryWest", L"Library Exit" },

            { L"Zone_Theatre", L"theatreSaveMain", L"Theatre Save" },
            { L"Zone_Theatre", L"theatreSouthWest", L"Theatre Pillar West Exit" },
            { L"Zone_Theatre", L"theatreSouthEast", L"Theatre Pillar East Exit" },
            { L"Zone_Theatre", L"theatreEast", L"Theatre Front Exit" },
            { L"Zone_Theatre", L"theatreNorthEastLower", L"Theatre Scythes North Exit" },
            { L"Zone_Theatre", L"theatreNorthEastUpper", L"Theatre Scythes South Exit" },

            { L"ZONE_Exterior", L"exteriorSouthSave", L"Bailey Save" },
            { L"ZONE_Exterior", L"lower1", L"Bailey North Exit" },
            { L"ZONE_Exterior", L"exteriorWest", L"Bailey West Exit" },
            { L"ZONE_Exterior", L"exteriorEast", L"Bailey Shack Exit" },
            { L"ZONE_Exterior", L"exteriorSouthEast", L"Bailey East Exit" },

            { L"Zone_Caves", L"cavesSouthSave", L"Underbelly South Save" },
            { L"Zone_Caves", L"cavesBigMiddleStart", L"Underbelly Central Save" },
            { L"Zone_Caves", L"cavesBigSideStart", L"Underbelly East Save" },
            { L"Zone_Caves", L"cavesWestSave", L"Underbelly Pre Light Save" },
            { L"Zone_Caves", L"postLightSave", L"Underbelly Post Light Save" },
            { L"Zone_Caves", L"cavesSouth", L"Underbelly South Exit" },
            { L"Zone_Caves", L"cavesEast", L"Underbelly Hole Exit" },
            { L"Zone_Caves", L"cavesWest", L"Underbelly Light Pillar Exit" },

            { L"Zone_Tower", L"towerSave", L"Tower Save" },
            { L"Zone_Tower", L"towerSouth", L"Tower South Exit" },
            { L"Zone_Tower", L"towerTop", L"Tower Top Exit" },

            { L"Zone_PrincessChambers", L"chambersStart", L"Chambers Exit" },
        };
        const size_t default_spawn_index = 7; // Castle West Save

        // map -> actor name -> location id + actor class name
        unordered_map<Map, unordered_map<wstring, Interactable>> interactable_table = {
            {Map::Dungeon, {
                {L"BP_NPC_C_1", {2365810066, L"BP_NPC_Child_C"}}, // Mirror Room Goatling
                {L"BP_NPC_C_6", {2365810067, L"BP_NPC_C"}}, // Rambling Goatling
                {L"BP_NPC_C_8", {2365810068, L"BP_NPC_C"}}, // Unwelcoming Goatling
                {L"BP_NPC_C_2", {2365810069, L"BP_NPC_C"}}, // Repentant Goatling
                {L"BP_NPC_C_4", {2365810070, L"BP_NPC_C"}}, // Defeatist Goatling
            }},
            {Map::Castle, {
                {L"BP_NPC_C_2", {2365810071, L"BP_NPC_C"}}, // Crystal Licker Goatling
                {L"BP_NPC_C_3", {2365810072, L"BP_NPC_C"}}, // Gazebo Goatling
                {L"BP_NPC_C_1", {2365810073, L"BP_NPC_C"}}, // Bubblephobic Goatling
                {L"BP_NPC_C_4", {2365810074, L"BP_NPC_C"}}, // Trapped Goatling
                {L"BP_NPC_C_5", {2365810075, L"BP_NPC_C"}}, // Memento Goatling
                {L"BP_NPC_Child_C_1", {2365810076, L"BP_NPC_Child_C"}}, // Goatling Near Library
                {L"BP_RestChair_C_3", {2365810085, L"BP_RestChair_C"}}, // Stool Near Crystal 1
                {L"BP_RestChair_C_1", {2365810086, L"BP_RestChair_C"}}, // Stool Near Crystal 2
                {L"BP_RestChair_C_2", {2365810087, L"BP_RestChair_C"}}, // Stool Near Crystal 3
                {L"BP_RestChair_C_4", {2365810088, L"BP_RestChair_C"}}, // Gazebo Stool
            }},
            {Map::Keep, {
                {L"BP_NPC_C_1", {2365810077, L"BP_NPC_C"}}, // Furniture-less Goatling
                {L"BP_NPC_C_2", {2365810078, L"BP_NPC_C"}}, // Distorted Goatling
                {L"BP_RestChair_C_1", {2365810089, L"BP_RestChair_C"}}, // Distorted Stool
                {L"BP_RestChair_C_3", {2365810090, L"BP_RestChair_C"}}, // Path to Throne Stool
                {L"BP_RestChair_C_4", {2365810091, L"BP_RestChair_C"}}, // The Throne
            }},
            {Map::Library, {
                {L"BP_RestChair_C_2", {2365810092, L"BP_RestChair_C"}}, // Hay Bale Near Entrance
                {L"BP_RestChair_C_3", {2365810093, L"BP_RestChair_C"}}, // Hay Bale Near Eggs
                {L"BP_RestChair_C_4", {2365810094, L"BP_RestChair_C"}}, // Hay Bale in the Back
                {L"BP_ExamineTextPopup_C_2", {2365810101, L"BP_ExamineTextPopup_C"}}, // A Book About a Princess
                {L"BP_ExamineTextPopup_C_3", {2365810102, L"BP_ExamineTextPopup_C"}}, // A Book About Cooking
                {L"BP_ExamineTextPopup_C_4", {2365810103, L"BP_ExamineTextPopup_C"}}, // A Book Full of Plays
                {L"BP_ExamineTextPopup_C_5", {2365810104, L"BP_ExamineTextPopup_C"}}, // A Book About Reading
                {L"BP_ExamineTextPopup_C_1", {2365810105, L"BP_ExamineTextPopup_C"}}, // A Book About Aquatic Life
                {L"BP_ExamineTextPopup_C_9", {2365810106, L"BP_ExamineTextPopup_C"}}, // A Book About a Jester
                {L"BP_ExamineTextPopup_C_0", {2365810107, L"BP_ExamineTextPopup_C"}}, // A Book About Loss
                {L"BP_ExamineTextPopup_C_7", {2365810108, L"BP_ExamineTextPopup_C"}}, // A Book on Musical Theory
                {L"BP_ExamineTextPopup_C_6", {2365810109, L"BP_ExamineTextPopup_C"}}, // A Book About a Girl
                {L"BP_ExamineTextPopup_C_8", {2365810110, L"BP_ExamineTextPopup_C"}}, // A Book About a Thimble
                {L"BP_Note_C_2", {2365810111, L"BP_ExamineTextPopup_C"}}, // A Book About a Monster
                {L"BP_Note_C_5", {2365810112, L"BP_ExamineTextPopup_C"}}, // A Book About Revenge
                {L"BP_ExamineTextPopup_C_10", {2365810113, L"BP_ExamineTextPopup_C"}}, // A Book About a Restaurant
                {L"BP_Note_C_3", {2365810114, L"BP_Note_C"}}, // Note Near Eggs
            }},
            {Map::Theatre, {
                {L"BP_NPC_C_10", {2365810079, L"BP_NPC_C"}}, // 20 Bean Casserole Goatling
                {L"BP_NPC_C_3", {2365810080, L"BP_NPC_C"}}, // Theatre Goer Goatling 1
                {L"BP_NPC_C_8", {2365810081, L"BP_NPC_C"}}, // Theatre Goer Goatling 2
                {L"BP_NPC_C_9", {2365810082, L"BP_NPC_C"}}, // Theatre Manager Goatling
                {L"BP_NPC_C_0", {2365810083, L"BP_NPC_C"}}, // Murderous Goatling
                {L"BP_RestChair_C_7", {2365810095, L"BP_RestChair_C"}}, // Stool Near Bookcase
                {L"BP_RestChair_C_4", {2365810096, L"BP_RestChair_C"}}, // Stool Around a Table 1
                {L"BP_RestChair_C_5", {2365810097, L"BP_RestChair_C"}}, // Stool Around a Table 2
                {L"BP_RestChair_C_1", {2365810098, L"BP_RestChair_C"}}, // Stool Around a Table 3
                {L"BP_RestChair_C_2", {2365810099, L"BP_RestChair_C"}}, // Stage Left Stool
                {L"BP_RestChair_C_3", {2365810100, L"BP_RestChair_C"}}, // Stage Right Stool
            }},
            {Map::Bailey, {
                {L"BP_NPC_C_1", {2365810084, L"BP_NPC_C"}}, // Alley Goatling
            }},
            {Map::Underbelly, {
                {L"BP_Note_C_7", {2365810115, L"BP_Note_C"}}, // Note on a Ledge
                {L"BP_Note_C_3", {2365810116, L"BP_Note_C"}}, // Note in the Big Room
                {L"BP_Note_C_5", {2365810117, L"BP_Note_C"}}, // Note Behind a Locked Door
            }},
        };

        // map -> note actor name -> text
        const unordered_map<Map, unordered_map<wstring, wstring>> note_text_table = {
            {Map::Library, {
                {L"BP_Note_C_3", L"I got some new shoes today, the seller said they're supposed to be great for parkour?\r\n\r\n\"These kicks will have you moving around like a laserbeam in a hall of mirrors!\r\n\r\nForget what you know,\r\ncarefully consider the angle of your jump before you make it.\r\nOnly by reflecting will you become a master of movement!\"\r\n\r\nHe really didn't need to give me all that though, i just thought they looked cool..."},
            }},
            {Map::Underbelly, {
                {L"BP_Note_C_7", L"someone is here.\r\n\r\nit's not just another part of the dream\r\n\r\nSOMEONE IS HERE"},
                {L"BP_Note_C_3", L"this was supposed to be an escape\r\n\r\ni didn't want ANYONE ELSE IN HERE!"},
                {L"BP_Note_C_5", L"STOP READING MY THOUGHTS\r\n\r\nWHO GAVE YOU THE RIGHT\r\n\r\n\r\n\r\nGET THE HELL OUT OF MY HEAD!!!"},
            }},
        };

        const unordered_map<wstring, Map> map_names = {
            {L"TitleScreen",            Map::TitleScreen},
            {L"ZONE_Dungeon",           Map::Dungeon},
            {L"ZONE_LowerCastle",       Map::Castle},
            {L"Zone_Upper",             Map::Keep},
            {L"Zone_Library",           Map::Library},
            {L"Zone_Theatre",           Map::Theatre},
            {L"ZONE_Exterior",          Map::Bailey},
            {L"Zone_Caves",             Map::Underbelly},
            {L"Zone_Tower",             Map::Tower},
            {L"Zone_PrincessChambers",  Map::Chambers},
            {L"EndScreen",              Map::EndScreen},
        };

        // maps tombstone actor name to major key index
        const unordered_map<wstring, int> major_key_tombstones_map = {
            {L"BP_ExamineTextPopup_C_2", 0},
            {L"BP_ExamineTextPopup_C_1", 1},
            {L"BP_ExamineTextPopup_C_3", 2},
            {L"BP_ExamineTextPopup_C_4", 3},
            {L"BP_ExamineTextPopup_C_5", 4},
        };

        // The two lookup tables below could be combined into one table with something like an ItemIdInfo struct,
        // and should be if a third table would be added.
        const unordered_map<int64_t, ItemType> lookup_item_id_to_type = {
            {2365810001, ItemType::MajorAbility},
            {2365810002, ItemType::MajorAbility},
            {2365810003, ItemType::MajorAbility},
            {2365810004, ItemType::MajorAbility},
            {2365810005, ItemType::MajorAbility},
            {2365810006, ItemType::MajorAbility},
            {2365810007, ItemType::MajorAbility},
            {2365810008, ItemType::MajorAbility},
            {2365810009, ItemType::MajorAbility},
            {2365810010, ItemType::MajorAbility},
            {2365810011, ItemType::MinorAbility},
            {2365810012, ItemType::MinorAbility},
            {2365810013, ItemType::MinorAbility},
            {2365810014, ItemType::MinorAbility},
            {2365810015, ItemType::MinorAbility},
            {2365810016, ItemType::MinorAbility},
            {2365810017, ItemType::MinorAbility},
            {2365810018, ItemType::MinorAbility},

            {2365810026, ItemType::MajorAbility},
            {2365810027, ItemType::MinorAbility},
            {2365810028, ItemType::MajorAbility},

            {2365810021, ItemType::MajorKey},
            {2365810022, ItemType::MajorKey},
            {2365810023, ItemType::MajorKey},
            {2365810024, ItemType::MajorKey},
            {2365810025, ItemType::MajorKey},

            {2365810019, ItemType::HealthPiece},
            {2365810020, ItemType::SmallKey},

            {2365810029, ItemType::MinorAbility},
            {2365810030, ItemType::MinorAbility},
            {2365810031, ItemType::MinorAbility},
            {2365810032, ItemType::MinorAbility},
            {2365810033, ItemType::MinorAbility},
            {2365810034, ItemType::MinorAbility},
            {2365810035, ItemType::MinorAbility},

            {2365810036, ItemType::MinorAbility},

            {2365810037, ItemType::MajorAbility},

            {2365810038, ItemType::Filler},
            {2365810039, ItemType::Filler},
        };

        // Upgrades starting with ~ don't actually exist in the game but are used to track AP items and are handled by
        // AP_SetUpgrades.
        const unordered_map<int64_t, wstring> lookup_item_id_to_upgrade = {
            {2365810001, L"attack"},
            {2365810002, L"powerBoost"},
            {2365810003, L"airKick"},
            {2365810004, L"slide"},
            // Maps to SlideJump. The set upgrades function may set SlideJump to 0 if solar wind is toggled off. By
            // tracking it separately, we can properly display whether solar wind has been obtained on file select.
            {2365810005, L"~solar"},
            {2365810006, L"plunge"},
            {2365810007, L"chargeAttack"},
            // Maps to wallRide. The wallRide upgrade grants the ability to cling, but the max number of clings is
            // stored in a separate variable, wallRideClingLimit. The set upgrades function will set wallRide to 1 if
            // any clings have been obtained, so we need to track cling gem separately.
            {2365810008, L"~cling6"},
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
            // Maps to slide and SlideJump based on the value of ~progressiveSlide. 
            {2365810026, L"~progressiveSlide"},
            {2365810027, L"extraKick"},
            // Maps to attack, chargeAttack, and projectile based on the value of ~progressiveBreaker.
            {2365810028, L"~progressiveBreaker"},
            {2365810029, L"outfitFaith"},
            {2365810030, L"outfitShoujo"},
            {2365810031, L"outfitSweater"},
            {2365810032, L"outfitClassy"},
            {2365810033, L"outfitKnight"},
            {2365810034, L"outfitJam"},
            {2365810035, L"outfitPast"},
            {2365810036, L"map"},
            // Maps to wallRide. See note about ~cling6 above.
            {2365810037, L"~cling2"},
        };
    } // End private members


    int GameData::GetHealthPieces() {
        return health_pieces;
    }

    int GameData::GetSmallKeys() {
        return small_keys;
    }

    bool* GameData::GetMajorKeys() {
        return major_keys;
    }

    unordered_map<wstring, int> GameData::GetUpgradeTable() {
        return upgrade_table;
    }

    void GameData::SetOption(string option_name, int value) {
        Log("Set option " + option_name + " to " + std::to_string(value));
        options[option_name] = value;
    }

    unordered_map<string, int> GameData::GetOptions() {
        return options;
    }

    unordered_map<int64_t, Collectible> GameData::GetCollectiblesOfZone(Map current_map) {
        return collectible_table[current_map];
    }

    unordered_map<wstring, Interactable> GetInteractablesOfZone(Map current_map) {
        return interactable_table[current_map];
    }

    list<int64_t> GameData::GetMissingSpawnableLocations() {
        list<int64_t> location_ids;
        for (const auto& [_, zone_map] : collectible_table) {
            for (const auto& [location_id, _] : zone_map) {
                if (Client::IsMissingLocation(location_id)) {
                    location_ids.push_back(location_id);
                }
            }
        }
        for (const auto& [_, zone_map] : time_trial_table) {
            for (const auto& [_, time_trial] : zone_map) {
                const int64_t location_id = time_trial.first;
                if (Client::IsMissingLocation(location_id)) {
                    location_ids.push_back(location_id);
                }
            }
        }
        if (Settings::GetInteractableAuraDisplay() == Settings::InteractableAuraDisplay::Classification) {
            for (const auto& [_, zone_map] : interactable_table) {
                for (const auto& [_, interactable] : zone_map) {
                    const int64_t location_id = interactable.first;
                    if (Client::IsMissingLocation(location_id)) {
                        location_ids.push_back(location_id);
                    }
                }
            }
        }
        return location_ids;
    }

    void GameData::SetPseudoItemClassification(int64_t location_id, int64_t item_id) {
        if (Settings::GetItemDisplay() == Settings::ItemDisplay::GenericAll) {
            return;
        }

        ItemType type = lookup_item_id_to_type.at(item_id);
        switch (type) {
        case ItemType::MajorAbility:
            lookup_location_id_to_classification[location_id] = Classification::MajorAbility;
            break;
        case ItemType::MinorAbility:
            lookup_location_id_to_classification[location_id] = Classification::MinorAbility;
            break;
        case ItemType::HealthPiece:
            lookup_location_id_to_classification[location_id] = Classification::HealthPiece;
            break;
        case ItemType::MajorKey:
            lookup_location_id_to_classification[location_id] = Classification::MajorKey;
            break;
        case ItemType::SmallKey:
            lookup_location_id_to_classification[location_id] = Classification::SmallKey;
            break;
        case ItemType::Filler:
            lookup_location_id_to_classification[location_id] = Classification::GenericFiller;
            break;
        }
    }

    void GameData::SetOffWorldItemClassification(int64_t location_id, Classification classification) {
        if (Settings::GetItemDisplay() == Settings::ItemDisplay::GenericNonPseudo ||
            Settings::GetItemDisplay() == Settings::ItemDisplay::GenericAll) {
            return;
        }

        lookup_location_id_to_classification[location_id] = classification;
    }

    Classification GameData::GetClassification(int64_t location_id) {
        if (lookup_location_id_to_classification.contains(location_id)) {
            return lookup_location_id_to_classification[location_id];
        }
        else {
            return Classification::Generic;
        }
    }

    void GameData::Initialize() {
        Close();

        collectible_table = {
            {Map::Dungeon, unordered_map<int64_t, Collectible>{
            // Dream Breaker
                {2365810001, Collectible(FVector(-3500.0, 4950.0, -50.0))},
            // Slide
                {2365810002, Collectible(FVector(16650, 2600, 2350))},
            // Alcove Near Mirror
                {2365810003, Collectible(FVector(1150, -400, 1050))},
            // Dark Orbs
                {2365810004, Collectible(FVector(18250, -9750, 4200))},
            // Past Poles
                {2365810005, Collectible(FVector(6800, 8850, 3850))},
            // Rafters
                {2365810006, Collectible(FVector(7487, 1407, 4250))},
            // Strong Eyes
                {2365810007, Collectible(FVector(750, 8850, 2650))},
                    }},
            {Map::Castle, unordered_map<int64_t, Collectible> {
            // Indignation
                {2365810008, Collectible(FVector(5400, 2100, -550))},
            // Alcove Near Dungeon
                {2365810009, Collectible(FVector(1600, 8000, -1400))},
            // Balcony
                {2365810010, Collectible(FVector(16400, 3800, 1200))},
            // Corner Corridor
                {2365810011, Collectible(FVector(11850, 1000, -300))},
            // Floater In Courtyard
                {2365810012, Collectible(FVector(-5000, -600, 2050))},
            // Locked Door
                {2365810013, Collectible(FVector(2700, -1700, -500))},
            // Platform In Main Halls
                {2365810014, Collectible(FVector(7950, 2750, -200))},
            // Tall Room Near Wheel Crawlers
                {2365810015, Collectible(FVector(-4100, -8200, 2950))},
            // Wheel Crawlers
                {2365810016, Collectible(FVector(-10050, -3700, 1000))},
            // High Climb From Courtyard
                {2365810017, Collectible(FVector(-3150, 11500, 6300))},
            // Alcove Near Scythe Corridor
                {2365810018, Collectible(FVector(-9600, 21750, 5400))},
            // Near Theatre Front
                {2365810019, Collectible(FVector(3390, 21150, 6600))},
            // Memento
                {2365810062, Collectible(FVector(8950, 6450, -175))},
                    }},
            {Map::Keep, unordered_map<int64_t, Collectible> {
            // Strikebreak
                {2365810020, Collectible(FVector(10050, 1800, 1000))},
            // Alcove Near Locked Door
                {2365810021, Collectible(FVector(800, 2500, 1200))},
            // Levers Room
                {2365810022, Collectible(FVector(1050, 15700, 1300))},
            // Lonely Throne
                {2365810023, Collectible(FVector(14350, -50, 1350))},
            // Near Theatre
                {2365810024, Collectible(FVector(-3900, -6109, -450))},
            // Sunsetter
                {2365810025, Collectible(FVector(-3000, 4900, -400))},
                    }},
            {Map::Library, unordered_map<int64_t, Collectible> {
            // Sun Greaves
                {2365810026, Collectible(FVector(-4150, 9200, -100))},
            // Upper Back
                {2365810027, Collectible(FVector(-9250, -1850, 1250))},
            // Locked Door Across
                {2365810028, Collectible(FVector(-1300, -6750, -700))},
            // Locked Door Left
                {2365810029, Collectible(FVector(-3750, -4170, -700))},
            // Split Greaves 1
                {2365810051, Collectible(FVector(-4150, 9160, 0))},
            // Split Greaves 2
                {2365810052, Collectible(FVector(-4100, 9250, -100))},
            // Split Greaves 3
                {2365810053, Collectible(FVector(-4200, 9250, -100))},
                    }},
            {Map::Theatre, unordered_map<int64_t, Collectible> {
            // Soul Cutter
                {2365810030, Collectible(FVector(8500, 7850, -1400))},
            // Back Of Auditorium
                {2365810031, Collectible(FVector(-1600, 1500, 2600))},
            // Center Stage
                {2365810032, Collectible(FVector(5200, 1550, 700))},
            // Locked Door
                {2365810033, Collectible(FVector(-1460, -2550, 2240))},
            // Tucked Behind Boxes
                {2365810034, Collectible(FVector(255, 1150, 50))},
            // Corner Beam
                {2365810035, Collectible(FVector(-14100, -150, 1950))},
                    }},
            {Map::Bailey, unordered_map<int64_t, Collectible> {
            // Solar Wind
                {2365810036, Collectible(FVector(-1100, 10850, 150))},
            // Center Steeple
                {2365810037, Collectible(FVector(2350, 7260, 2110))},
            // Cheese Bell
                {2365810038, Collectible(FVector(5040, 7150, 2500))},
            // Guarded Hand
                {2365810039, Collectible(FVector(-1787, 5236, 650))},
            // Inside Building
                {2365810040, Collectible(FVector(3007, 3457, 300))},
                    }},
            {Map::Underbelly, unordered_map<int64_t, Collectible> {
            // Ascendant Light
                {2365810041, Collectible(FVector(-5400, 6650, 6750))},
            // Alcove Near Light
                {2365810042, Collectible(FVector(-2550, 12300, 4400))},
            // Building Near Little Guy
                {2365810043, Collectible(FVector(-4350, 28350, 1850))},
            // Locked Door
                {2365810044, Collectible(FVector(18896, 7937, 1200))},
            // Main Room
                {2365810045, Collectible(FVector(-726, 19782, 3200))},
            // Rafters Near Keep
                {2365810046, Collectible(FVector(19600, 17750, 5700))},
            // Strikebreak Wall
                {2365810047, Collectible(FVector(11300, 12700, 3107))},
            // Surrounded By Holes
                {2365810048, Collectible(FVector(33050, 24100, 3850), tuple<FVector, string, int>{FVector(31900, 26250, 3850), "game_version", FULL_GOLD})},
                    }},
            {Map::Tower, unordered_map<int64_t, Collectible> {
            // Cling Gem
                {2365810049, Collectible(FVector(13350, 5250, 4150))},
            // Atop The Tower
                {2365810050, Collectible(FVector(9650, 5250, 7100))},
            // Cling Gem 1
                {2365810063, Collectible(FVector(13350, 4750, 4150))},
            // Cling Gem 2
                {2365810064, Collectible(FVector(13350, 5250, 4150))},
            // Cling Gem 3
                {2365810065, Collectible(FVector(13350, 5750, 4150))},
                    }},
        };

        // map -> time trial actor name -> location id + position
        time_trial_table = {
            {Map::Dungeon, {
                {L"BP_TimeTrial_C_1", {2365810054, FVector(-3350, -4300, 850)}},
            }},
            {Map::Castle, {
                {L"BP_TimeTrial_C_1", {2365810055, FVector(3200, -1700, -500)}},
            }},
            {Map::Keep, {
                {L"BP_TimeTrial_C_3", {2365810056, FVector(14350, 400, 1250)}},
            }},
            {Map::Library, {
                {L"BP_TimeTrial_C_2", {2365810057, FVector(-2850, 3600, 900)}},
            }},
            {Map::Theatre, {
                {L"BP_TimeTrial_C_1", {2365810058, FVector(-14750, 3900, 100)}},
            }},
            {Map::Bailey, {
                {L"BP_TimeTrial_C_1", {2365810059, FVector(1150, 5250, -600)}},
            }},
            {Map::Underbelly, {
                {L"BP_TimeTrial_C_1", {2365810060, FVector(1250, 18000, 3000)}},
            }},
            {Map::Tower, {
                {L"BP_TimeTrial_C_3", {2365810061, FVector(10750, 3050, 4000)}},
            }},
        };
    }

    void GameData::Close() {
        collectible_table = {};
        time_trial_table = {};
        ResetItems();
        for (auto& hints : major_key_hints) {
            hints.clear();
        }
    }

    void ResetItems() {
        slidejump_owned = false;
        slidejump_disabled = false;
        small_keys = 0;
        health_pieces = 0;
        for (bool &k : major_keys) {
            k = false;
        }
        upgrade_table = {};
    }

    ItemType GameData::ReceiveItem(int64_t id) {
        ItemType type = lookup_item_id_to_type.at(id);
        switch (type) {
        case ItemType::MajorAbility:
        case ItemType::MinorAbility:
            upgrade_table[lookup_item_id_to_upgrade.at(id)]++;
            if (!slidejump_owned && (upgrade_table[L"slide"] && upgrade_table[L"~solar"]
                                     || upgrade_table[L"~progressiveSlide"] >= 2)) {
                slidejump_owned = true;
            }
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
        case ItemType::Filler:
            // filler does something immediate, so it should only be handled when receiving a PrintJSON with the item
            break;
        default:
            Log(L"You were sent an item, but its id wasn't recognized. Verify that you're playing on the same version this seed was generated on.");
            break;
        }
        return type;
    }

    Map GameData::MapNameToEnum(wstring map_name) {
        return map_names.at(map_name);
    }

    bool GameData::ToggleSlideJump() {
        if (!slidejump_owned) {
            Log(L"Slidejump is not obtained");
            return false;
        }

        slidejump_disabled = !slidejump_disabled;
        if (slidejump_disabled) {
            Log(L"Solar wind is now OFF.");
        }
        else {
            Log(L"Solar wind is now ON.");
        }
        return true;
    }

    bool GameData::SlideJumpDisabled() {
        return slidejump_disabled;
    }

    bool CanHaveTimeTrial(Map map) {
        switch (map) {
        case Map::Dungeon:
        case Map::Castle:
        case Map::Keep:
        case Map::Library:
        case Map::Theatre:
        case Map::Bailey:
        case Map::Underbelly:
        case Map::Tower:
            return true;
        }
        return false;
    }

    optional<TimeTrial> GetTimeTrial(Map map, wstring time_trial_actor_name) {
        if (!time_trial_table.contains(map)) {
            return {};
        }
        if (!time_trial_table.at(map).contains(time_trial_actor_name)) {
            return {};
        }
        return time_trial_table.at(map).at(time_trial_actor_name);
    }

    void Interact(wstring actor_name) {
        std::optional<Interactable> interactable = GameData::GetInteractable(actor_name);
        if (!interactable) {
            Log(L"No interactable found for interactable actor " + actor_name); // TODO add zone to log
            return;
        }
        int64_t location_id = interactable->first;
        if (!Client::IsMissingLocation(location_id)) {
            Log(L"Interactable location " + std::to_wstring(location_id) + L" is not missing.");
            return;
        }
        Log(L"Sending check for interactable location " + std::to_wstring(location_id));
        Client::SendCheck(location_id);
    }

    void ReadNote(wstring name) {
        Log(L"Note being read: " + name);
        note_being_read = name;
    }

    void FinishNote() {
        if (!note_being_read) {
            Log(L"No note is being read.");
            return;
        }

        Log(L"Note finished: " + *note_being_read);
        Interact(*note_being_read);
        note_being_read = {};
    }

    optional<wstring> GetNoteText(wstring note_actor_name) {
        Map map = Engine::GetCurrentMap();
        if (!note_text_table.contains(map)) {
            return {};
        }
        if (!note_text_table.at(map).contains(note_actor_name)) {
            return {};
        }
        return note_text_table.at(map).at(note_actor_name);
    }

    void ReceiveItemOnce(int64_t item_id) {
        switch (item_id) {
        case 2365810038: // Healing
            Engine::HealPlayer();
            break;
        case 2365810039: // Magic Power
            Engine::GivePlayerPower();
            break;
        }
    }

    bool IsInteractable(int64_t location_id) {
        // this works for now since locations are separated by collectible/interactable at this location id
        return location_id >= 2365810066;
    }

    void AddMajorKeyHint(int key_index, MultiworldLocation hint) {
        if (key_index < 0 || key_index >= 5) {
            Log("Tried to add a key hint out of the allowed range [0-5): index " + std::to_string(key_index));
            return;
        }
        Log("Adding hint for key " + std::to_string(key_index) + ": player " + std::to_string(hint.player_id) +
            ", location " + std::to_string(hint.location_id));
        major_key_hints[key_index].push_back(hint);
    }

    optional<MajorKeyInfo> GetMajorKeyInfo(wstring tombstone_actor_name) {
        if (!major_key_tombstones_map.contains(tombstone_actor_name)) {
            return {};
        }

        int index = major_key_tombstones_map.at(tombstone_actor_name);
        int64_t item_id = index + 2365810021;
        bool found = major_keys[index];
        vector<MultiworldLocation> hints = major_key_hints[index];
        return MajorKeyInfo{ item_id, found, hints };
    }

    const PlayerStart& GetSpawnInfo() {
        if (!options.contains("spawn_point")) {
            return player_starts.at(default_spawn_index);
        }

        const int& spawn_point = options.at("spawn_point");
        if (spawn_point < 0 || spawn_point >= player_starts.size()) {
            return player_starts.at(default_spawn_index);
        }

        return player_starts.at(spawn_point);
    }


    namespace {
        optional<Interactable> GetInteractable(wstring interactable_actor_name) {
            Map map = Engine::GetCurrentMap();
            if (!interactable_table.contains(map)) {
                return {};
            }
            if (!interactable_table.at(map).contains(interactable_actor_name)) {
                return {};
            }
            return interactable_table.at(map).at(interactable_actor_name);
        }
    }
}