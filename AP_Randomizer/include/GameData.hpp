#pragma once
#include <map>
#include <vector>
#include "Collectible.hpp"

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
	std::vector<Collectible> GetCollectiblesOfZone(std::wstring);
	void CheckLocation(int64_t);
	ItemType ReceiveItem(int64_t);
}