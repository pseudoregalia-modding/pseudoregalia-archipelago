#pragma once
#include "DynamicOutput/DynamicOutput.hpp"
#include "Archipelago.h"
#include "APGameManager.hpp"
#include "APCollectible.hpp"

#include <locale>
#include <codecvt>
#include <string>

namespace Pseudoregalia_AP {
	enum class ItemType {
		Ability,
		MinorUpgrade,
		HealthPiece,
		SmallKey,
		MajorKey,
		Unknown
	};

	class APClient {
	public:
		static void Initialize();
		static void Connect(const char*, const char*, const char*);
		static void SendCheck(int64_t, std::wstring);
		static void PollServer();
		static std::map<std::wstring, int> GetUpgradeTable();
		static std::vector<APCollectible> GetCurrentZoneCollectibles(std::wstring);
		static bool* GetMajorKeys();
		static int GetSmallKeys();
		static int GetHealthPieces();
		static void CompleteGame();
	private:
		static std::string slot_name;
		static void FillZoneTable();
		static void ResetUpgradeTable();
		static void ClearItems();
		static void ReceiveItem(int64_t, bool);
		static void CheckLocation(int64_t);
		static void CheckLocation(APCollectible&, std::wstring);
		static bool ConnectionStatusChanged();
		static ItemType GetItemType(int64_t);
		static std::map<int64_t, std::wstring> lookup_id_to_item;
		static std::map <std::wstring, std::vector<APCollectible>> zone_table;
		static std::map<std::wstring, int> upgrade_table;
		static bool major_keys[5];
		static int small_keys;
		static int health_pieces;
		static int connection_timer;
		static AP_ConnectionStatus connection_status;
	};
}