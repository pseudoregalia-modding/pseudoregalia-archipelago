#pragma once
#include <map>
#include <vector>
#include <string>
#include "APCollectible.hpp"

namespace Pseudoregalia_AP {
	enum class ItemType {
		Ability,
		HealthPiece,
		SmallKey,
		MajorKey,
		Unknown
	};

	class GameData {
	public:
		static void Initialize();
		static int GetHealthPieces();
		static int GetSmallKeys();
		static bool* GetMajorKeys();
		static std::map<std::wstring, int> GetUpgradeTable();
		static std::vector<APCollectible> GetCollectiblesOfZone(std::wstring);
		static void CheckLocation(int64_t);
		static void ReceiveItem(int64_t);
	private:
		static ItemType GetItemType(int64_t);
		static int health_pieces;
		static int small_keys;
		static bool major_keys[5];
		const static std::map<int64_t, std::wstring> lookup_id_to_upgrade;
		static std::map<std::wstring, int> upgrade_table;
		static std::map<std::wstring, std::vector<APCollectible>> collectible_table;
	};
}