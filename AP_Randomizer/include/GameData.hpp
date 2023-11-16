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
	void SetOption(std::string, int);
	std::unordered_map<std::string, int> GetOptions();
	std::unordered_map<std::wstring, int> GetUpgradeTable();
	std::unordered_map<int64_t, Collectible> GetCollectiblesOfZone(std::wstring);
	void CheckLocation(const int64_t);
	ItemType ReceiveItem(int64_t);
	bool ToggleSlideJump();
	bool SlideJumpDisabled();
}