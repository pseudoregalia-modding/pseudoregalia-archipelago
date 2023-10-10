#pragma once
#include <map>
#include <vector>
#include <string>
#include "APCollectible.hpp"

using namespace RC::Unreal;

namespace GameData {
	void Initialize();
	int GetHealthPieces();
	int GetSmallKeys();
	bool* GetMajorKeys();
	std::map<std::wstring, int> GetUpgradeTable();
	std::vector<APCollectible> GetCollectiblesOfZone(std::wstring);
	void CheckLocation(int64_t);
	void ReceiveItem(int64_t);
}