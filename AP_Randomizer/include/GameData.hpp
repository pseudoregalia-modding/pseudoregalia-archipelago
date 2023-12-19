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

	enum class Map {
		TitleScreen,
		Dungeon,
		Castle,
		Keep,
		Library,
		Theatre,
		Bailey,
		Underbelly,
		Tower,
		Chambers,
		EndScreen,
	};

	void Initialize();
	void Close();
	int GetHealthPieces();
	int GetSmallKeys();
	bool* GetMajorKeys();
	void SetOption(std::string, int);
	std::unordered_map<std::string, int> GetOptions();
	std::unordered_map<std::wstring, int> GetUpgradeTable();
	std::unordered_map<int64_t, Collectible> GetCollectiblesOfZone(Map);
	void CheckLocation(const int64_t);
	ItemType ReceiveItem(int64_t);
	Map MapNameToEnum(std::wstring);
	bool ToggleSlideJump();
	bool SlideJumpDisabled();
}