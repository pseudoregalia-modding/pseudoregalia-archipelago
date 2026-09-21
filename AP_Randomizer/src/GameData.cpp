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
        optional<Interactable> GetInteractable(wstring);

        optional<wstring> note_being_read = {};

        int health_pieces;
        int small_keys;
        bool major_keys[5];
        vector<MultiworldLocation> major_key_hints[5];
        unordered_map<wstring, int> upgrade_table;
        unordered_map<int64_t, ItemType> lookup_location_id_to_item_type;
        unordered_map<string, int> options;
        bool slidejump_owned;
        bool slidejump_disabled;

        const unordered_map<Map, unordered_map<int64_t, Position>> collectible_table = {
            {Map::Dungeon, unordered_map<int64_t, Position>{
                {1, { -3500, 4950, -50 }},     // Dream Breaker
                {2, { 16650, 2600, 2350 }},    // Slide
                {3, { 1150, -400, 1050 }},     // Alcove Near Mirror
                {4, { 18250, -9750, 4200 }},   // Dark Orbs
                {5, { 6800, 8850, 3850 }},     // Past Poles
                {6, { 7487, 1407, 4250 }},     // Rafters
                {7, { 750, 8850, 2650  }},     // Strong Eyes
            }},
            {Map::Castle, unordered_map<int64_t, Position> {
                {8, { 5400, 2100, -550 }},     // Indignation
                {9, { 1600, 8000, -1400 }},    // Alcove Near Dungeon
                {10, { 16400, 3800, 1200 }},   // Balcony
                {11, { 11850, 1000, -300 }},   // Corner Corridor
                {12, { -5000, -600, 2050 }},   // Floater In Courtyard
                {14, { 7950, 2750, -200 }},    // Platform In Main Halls
                {15, { -4100, -8200, 2950 }},  // Tall Room Near Wheel Crawlers
                {16, { -10050, -3700, 1000 }}, // Wheel Crawlers
                {17, { -3150, 11500, 6300 }},  // High Climb From Courtyard
                {18, { -9600, 21750, 5400 }},  // Alcove Near Scythe Corridor
                {19, { 3390, 21150, 6600 }},   // Near Theatre Front
                {62, { 8950, 6450, -175 }},    // Memento
            }},
            {Map::Keep, unordered_map<int64_t, Position> {
                {20, { 10050, 1800, 1000 }},   // Strikebreak
                {21, { 800, 2500, 1200 }},     // Alcove Near Locked Door
                {22, { 1050, 15700, 1300 }},   // Levers Room
                {23, { 14350, -50, 1350 }},    // Lonely Throne
                {24, { -3900, -6109, -450 }},  // Near Theatre
                {25, { -3000, 4900, -400 }},   // Sunsetter
            }},
            {Map::Library, unordered_map<int64_t, Position> {
                {26, { -4150, 9200, -100 }},   // Sun Greaves
                {27, { -9250, -1850, 1250 }},  // Upper Back
                {28, { -1300, -6750, -700 }},  // Locked Door Across
                {29, { -3750, -4170, -700 }},  // Locked Door Left
                {51, { -4150, 9160, 0 }},      // Split Greaves 1
                {52, { -4100, 9250, -100 }},   // Split Greaves 2
                {53, { -4200, 9250, -100 }},   // Split Greaves 3
            }},
            {Map::Theatre, unordered_map<int64_t, Position> {
                {30, { 8500, 7850, -1400 }},   // Soul Cutter
                {31, { -1600, 1500, 2600 }},   // Back Of Auditorium
                {32, { 5200, 1550, 700 }},     // Center Stage
                {33, { -1460, -2550, 2240 }},  // Locked Door
                {34, { 255, 1150, 50 }},       // Tucked Behind Boxes
                {35, { -14100, -150, 1950 }},  // Corner Beam
            }},
            {Map::Bailey, unordered_map<int64_t, Position> {
                {36, { -1100, 10850, 150 }},   // Solar Wind
                {37, { 2350, 7260, 2110 }},    // Center Steeple
                {38, { 5040, 7150, 2500 }},    // Cheese Bell
                {39, { -1787, 5236, 650 }},    // Guarded Hand
                {40, { 3007, 3457, 300 }},     // Inside Building
            }},
            {Map::Underbelly, unordered_map<int64_t, Position> {
                {41, { -5400, 6650, 6750 }},   // Ascendant Light
                {42, { -2550, 12300, 4400 }},  // Alcove Near Light
                {43, { -4350, 28350, 1850 }},  // Building Near Little Guy
                {44, { 18896, 7937, 1200 }},   // Locked Door
                {45, { -726, 19782, 3200 }},   // Main Room
                {46, { 19600, 17750, 5700 }},  // Rafters Near Keep
                {47, { 11300, 12700, 3107 }},  // Strikebreak Wall
                {48, { 33050, 24100, 3850 }},  // Surrounded By Holes
            }},
            {Map::Tower, unordered_map<int64_t, Position> {
                {49, { 13350, 5250, 4150 }},   // Cling Gem
                {50, { 9650, 5250, 7100 }},    // Atop The Tower
                {63, { 13350, 4750, 4150 }},   // Cling Gem 1
                {64, { 13350, 5250, 4150 }},   // Cling Gem 2
                {65, { 13350, 5750, 4150 }},   // Cling Gem 3
            }},
        };

        // map -> time trial actor name -> location id + position
        const unordered_map<Map, unordered_map<wstring, TimeTrial>> time_trial_table = {
            {Map::Dungeon, {
                {L"BP_TimeTrial_C_1", {54, { -3350, -4300, 850 }}},
            }},
            {Map::Castle, {
                {L"BP_TimeTrial_C_1", {55, { 3200, -1700, -500 }}},
            }},
            {Map::Keep, {
                {L"BP_TimeTrial_C_3", {56, { 14350, 400, 1250 }}},
            }},
            {Map::Library, {
                {L"BP_TimeTrial_C_2", {57, { -2850, 3600, 900 }}},
            }},
            {Map::Theatre, {
                {L"BP_TimeTrial_C_1", {58, { -14750, 3900, 100 }}},
            }},
            {Map::Bailey, {
                {L"BP_TimeTrial_C_1", {59, { 1150, 5250, -600 }}},
            }},
            {Map::Underbelly, {
                {L"BP_TimeTrial_C_1", {60, { 1250, 18000, 3000 }}},
            }},
            {Map::Tower, {
                {L"BP_TimeTrial_C_3", {61, { 10750, 3050, 4000 }}},
            }},
        };

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
        const unordered_map<Map, unordered_map<wstring, Interactable>> interactable_table = {
            {Map::Dungeon, {
                {L"BP_NPC_C_1", {66, L"BP_NPC_Child_C"}}, // Mirror Room Goatling
                {L"BP_NPC_C_6", {67, L"BP_NPC_C"}}, // Rambling Goatling
                {L"BP_NPC_C_8", {68, L"BP_NPC_C"}}, // Unwelcoming Goatling
                {L"BP_NPC_C_2", {69, L"BP_NPC_C"}}, // Repentant Goatling
                {L"BP_NPC_C_4", {70, L"BP_NPC_C"}}, // Defeatist Goatling
            }},
            {Map::Castle, {
                {L"BP_NPC_C_2", {71, L"BP_NPC_C"}}, // Crystal Licker Goatling
                {L"BP_NPC_C_3", {72, L"BP_NPC_C"}}, // Gazebo Goatling
                {L"BP_NPC_C_1", {73, L"BP_NPC_C"}}, // Bubblephobic Goatling
                {L"BP_NPC_C_4", {74, L"BP_NPC_C"}}, // Trapped Goatling
                {L"BP_NPC_C_5", {75, L"BP_NPC_C"}}, // Memento Goatling
                {L"BP_NPC_Child_C_1", {76, L"BP_NPC_Child_C"}}, // Goatling Near Library
                {L"BP_RestChair_C_3", {85, L"BP_RestChair_C"}}, // Stool Near Crystal 1
                {L"BP_RestChair_C_1", {86, L"BP_RestChair_C"}}, // Stool Near Crystal 2
                {L"BP_RestChair_C_2", {87, L"BP_RestChair_C"}}, // Stool Near Crystal 3
                {L"BP_RestChair_C_4", {88, L"BP_RestChair_C"}}, // Gazebo Stool
            }},
            {Map::Keep, {
                {L"BP_NPC_C_1", {77, L"BP_NPC_C"}}, // Furniture-less Goatling
                {L"BP_NPC_C_2", {78, L"BP_NPC_C"}}, // Distorted Goatling
                {L"BP_RestChair_C_1", {89, L"BP_RestChair_C"}}, // Distorted Stool
                {L"BP_RestChair_C_3", {90, L"BP_RestChair_C"}}, // Path to Throne Stool
                {L"BP_RestChair_C_4", {91, L"BP_RestChair_C"}}, // The Throne
            }},
            {Map::Library, {
                {L"BP_RestChair_C_2", {92, L"BP_RestChair_C"}}, // Hay Bale Near Entrance
                {L"BP_RestChair_C_3", {93, L"BP_RestChair_C"}}, // Hay Bale Near Eggs
                {L"BP_RestChair_C_4", {94, L"BP_RestChair_C"}}, // Hay Bale in the Back
                {L"BP_ExamineTextPopup_C_2", {101, L"BP_ExamineTextPopup_C"}}, // A Book About a Princess
                {L"BP_ExamineTextPopup_C_3", {102, L"BP_ExamineTextPopup_C"}}, // A Book About Cooking
                {L"BP_ExamineTextPopup_C_4", {103, L"BP_ExamineTextPopup_C"}}, // A Book Full of Plays
                {L"BP_ExamineTextPopup_C_5", {104, L"BP_ExamineTextPopup_C"}}, // A Book About Reading
                {L"BP_ExamineTextPopup_C_1", {105, L"BP_ExamineTextPopup_C"}}, // A Book About Aquatic Life
                {L"BP_ExamineTextPopup_C_9", {106, L"BP_ExamineTextPopup_C"}}, // A Book About a Jester
                {L"BP_ExamineTextPopup_C_0", {107, L"BP_ExamineTextPopup_C"}}, // A Book About Loss
                {L"BP_ExamineTextPopup_C_7", {108, L"BP_ExamineTextPopup_C"}}, // A Book on Musical Theory
                {L"BP_ExamineTextPopup_C_6", {109, L"BP_ExamineTextPopup_C"}}, // A Book About a Girl
                {L"BP_ExamineTextPopup_C_8", {110, L"BP_ExamineTextPopup_C"}}, // A Book About a Thimble
                {L"BP_Note_C_2", {111, L"BP_ExamineTextPopup_C"}}, // A Book About a Monster
                {L"BP_Note_C_5", {112, L"BP_ExamineTextPopup_C"}}, // A Book About Revenge
                {L"BP_ExamineTextPopup_C_10", {113, L"BP_ExamineTextPopup_C"}}, // A Book About a Restaurant
                {L"BP_Note_C_3", {114, L"BP_Note_C"}}, // Note Near Eggs
            }},
            {Map::Theatre, {
                {L"BP_NPC_C_10", {79, L"BP_NPC_C"}}, // 20 Bean Casserole Goatling
                {L"BP_NPC_C_3", {80, L"BP_NPC_C"}}, // Theatre Goer Goatling 1
                {L"BP_NPC_C_8", {81, L"BP_NPC_C"}}, // Theatre Goer Goatling 2
                {L"BP_NPC_C_9", {82, L"BP_NPC_C"}}, // Theatre Manager Goatling
                {L"BP_NPC_C_0", {83, L"BP_NPC_C"}}, // Murderous Goatling
                {L"BP_RestChair_C_7", {95, L"BP_RestChair_C"}}, // Stool Near Bookcase
                {L"BP_RestChair_C_4", {96, L"BP_RestChair_C"}}, // Stool Around a Table 1
                {L"BP_RestChair_C_5", {97, L"BP_RestChair_C"}}, // Stool Around a Table 2
                {L"BP_RestChair_C_1", {98, L"BP_RestChair_C"}}, // Stool Around a Table 3
                {L"BP_RestChair_C_2", {99, L"BP_RestChair_C"}}, // Stage Left Stool
                {L"BP_RestChair_C_3", {100, L"BP_RestChair_C"}}, // Stage Right Stool
            }},
            {Map::Bailey, {
                {L"BP_NPC_C_1", {84, L"BP_NPC_C"}}, // Alley Goatling
            }},
            {Map::Underbelly, {
                {L"BP_Note_C_7", {115, L"BP_Note_C"}}, // Note on a Ledge
                {L"BP_Note_C_3", {116, L"BP_Note_C"}}, // Note in the Big Room
                {L"BP_Note_C_5", {117, L"BP_Note_C"}}, // Note Behind a Locked Door
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
        const unordered_map<int64_t, EPseudoType::Type> lookup_item_id_to_type = {
            {1, EPseudoType::Type::MajorAbility},
            {2, EPseudoType::Type::MajorAbility},
            {3, EPseudoType::Type::MajorAbility},
            {4, EPseudoType::Type::MajorAbility},
            {5, EPseudoType::Type::MajorAbility},
            {6, EPseudoType::Type::MajorAbility},
            {7, EPseudoType::Type::MajorAbility},
            {8, EPseudoType::Type::MajorAbility},
            {9, EPseudoType::Type::MajorAbility},
            {10, EPseudoType::Type::MajorAbility},
            {11, EPseudoType::Type::MinorAbility},
            {12, EPseudoType::Type::MinorAbility},
            {13, EPseudoType::Type::MinorAbility},
            {14, EPseudoType::Type::MinorAbility},
            {15, EPseudoType::Type::MinorAbility},
            {16, EPseudoType::Type::MinorAbility},
            {17, EPseudoType::Type::MinorAbility},
            {18, EPseudoType::Type::MinorAbility},

            {26, EPseudoType::Type::MajorAbility},
            {27, EPseudoType::Type::MinorAbility},
            {28, EPseudoType::Type::MajorAbility},

            {21, EPseudoType::Type::MajorKey},
            {22, EPseudoType::Type::MajorKey},
            {23, EPseudoType::Type::MajorKey},
            {24, EPseudoType::Type::MajorKey},
            {25, EPseudoType::Type::MajorKey},

            {19, EPseudoType::Type::HealthPiece},
            {20, EPseudoType::Type::SmallKey},

            {29, EPseudoType::Type::MinorAbility},
            {30, EPseudoType::Type::MinorAbility},
            {31, EPseudoType::Type::MinorAbility},
            {32, EPseudoType::Type::MinorAbility},
            {33, EPseudoType::Type::MinorAbility},
            {34, EPseudoType::Type::MinorAbility},
            {35, EPseudoType::Type::MinorAbility},

            {36, EPseudoType::Type::MinorAbility},

            {37, EPseudoType::Type::MajorAbility},

            {38, EPseudoType::Type::OffWorld},
            {39, EPseudoType::Type::OffWorld},
        };

        // Upgrades starting with ~ don't actually exist in the game but are used to track AP items and are handled by
        // AP_SetUpgrades.
        const unordered_map<int64_t, wstring> lookup_item_id_to_upgrade = {
            {1, L"attack"},
            {2, L"powerBoost"},
            {3, L"airKick"},
            {4, L"slide"},
            // Maps to SlideJump. The set upgrades function may set SlideJump to 0 if solar wind is toggled off. By
            // tracking it separately, we can properly display whether solar wind has been obtained on file select.
            {5, L"~solar"},
            {6, L"plunge"},
            {7, L"chargeAttack"},
            // Maps to wallRide. The wallRide upgrade grants the ability to cling, but the max number of clings is
            // stored in a separate variable, wallRideClingLimit. The set upgrades function will set wallRide to 1 if
            // any clings have been obtained, so we need to track cling gem separately.
            {8, L"~cling6"},
            {9, L"Light"},
            {10, L"projectile"},
            {11, L"extraKick"},
            {12, L"airRecovery"},
            {13, L"mobileHeal"},
            {14, L"magicHaste"},
            {15, L"healBoost"},
            {16, L"damageBoost"},
            {17, L"magicPiece"},
            {18, L"outfitPro"},
            // Maps to slide and SlideJump based on the value of ~progressiveSlide. 
            {26, L"~progressiveSlide"},
            {27, L"extraKick"},
            // Maps to attack, chargeAttack, and projectile based on the value of ~progressiveBreaker.
            {28, L"~progressiveBreaker"},
            {29, L"outfitFaith"},
            {30, L"outfitShoujo"},
            {31, L"outfitSweater"},
            {32, L"outfitClassy"},
            {33, L"outfitKnight"},
            {34, L"outfitJam"},
            {35, L"outfitPast"},
            {36, L"map"},
            // Maps to wallRide. See note about ~cling6 above.
            {37, L"~cling2"},
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

    unordered_map<int64_t, Position> GameData::GetCollectiblesOfZone(Map current_map) {
        if (collectible_table.contains(current_map)) {
            return collectible_table.at(current_map);
        }
        return {};
    }

    unordered_map<wstring, Interactable> GetInteractablesOfZone(Map current_map) {
        if (interactable_table.contains(current_map)) {
            return interactable_table.at(current_map);
        }
        return {};
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
        for (const auto& [_, zone_map] : interactable_table) {
            for (const auto& [_, interactable] : zone_map) {
                const int64_t location_id = interactable.first;
                if (Client::IsMissingLocation(location_id)) {
                    location_ids.push_back(location_id);
                }
            }
        }
        return location_ids;
    }

    void GameData::SetPseudoItemType(int64_t location_id, int64_t item_id, EClassification::Type classification) {
        lookup_location_id_to_item_type[location_id] = { lookup_item_id_to_type.at(item_id), classification };
    }

    void GameData::SetOffWorldItemType(int64_t location_id, EClassification::Type classification) {
        lookup_location_id_to_item_type[location_id] = { EPseudoType::Type::OffWorld, classification };
    }

    ItemType GameData::GetItemType(int64_t location_id) {
        if (lookup_location_id_to_item_type.contains(location_id)) {
            return lookup_location_id_to_item_type[location_id];
        }
        else {
            return { EPseudoType::Type::OffWorld, EClassification::Unknown };
        }
    }

    void GameData::Close() {
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

    void GameData::ReceiveItem(int64_t id) {
        Log(L"Receiving item with id " + std::to_wstring(id));

        EPseudoType::Type type = lookup_item_id_to_type.at(id);
        switch (type) {
        case EPseudoType::Type::MajorAbility:
        case EPseudoType::Type::MinorAbility:
            upgrade_table[lookup_item_id_to_upgrade.at(id)]++;
            if (!slidejump_owned && (upgrade_table[L"slide"] && upgrade_table[L"~solar"]
                                     || upgrade_table[L"~progressiveSlide"] >= 2)) {
                slidejump_owned = true;
            }
            break;
        case EPseudoType::Type::HealthPiece:
            health_pieces++;
            break;
        case EPseudoType::Type::SmallKey:
            small_keys++;
            break;
        case EPseudoType::Type::MajorKey:
            // Remove prefix digits from id to assign directly to major_keys array
            major_keys[id - 21] = true;
            break;
        case EPseudoType::Type::OffWorld:
            switch (id) {
            case 38: // Healing
                Engine::HealPlayer();
                break;
            case 39: // Magic Power
                Engine::GivePlayerPower();
                break;
            }
            break;
        default:
            Log(L"You were sent an item, but its id wasn't recognized. Verify that you're playing on the same version this seed was generated on.");
            break;
        }
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
        auto interactable = GameData::GetInteractable(actor_name);
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

    bool IsInteractable(int64_t location_id) {
        // this works for now since locations are separated by collectible/interactable at this location id
        return location_id >= 66;
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
        int64_t item_id = index + 21;
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