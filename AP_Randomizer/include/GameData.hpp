#pragma once
#include <map>
#include <vector>
#include <string>
#include "APCollectible.hpp"

using namespace RC::Unreal;

namespace GameData {
	enum class ItemType {
		Ability,
		HealthPiece,
		SmallKey,
		MajorKey,
		Unknown
	};

	void Initialize();
	int GetHealthPieces();
	int GetSmallKeys();
	bool* GetMajorKeys();
	std::map<std::wstring, int> GetUpgradeTable();
	std::vector<APCollectible> GetCollectiblesOfZone(std::wstring);
	void CheckLocation(int64_t);
	void ReceiveItem(int64_t);


	ItemType GetItemType(int64_t);
	int health_pieces;
	int small_keys;
	bool major_keys[5];
	const std::map<int64_t, std::wstring> lookup_id_to_upgrade;
	std::map<std::wstring, int> upgrade_table;
	std::map<std::wstring, std::vector<APCollectible>> collectible_table;
}