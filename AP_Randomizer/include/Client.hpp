#pragma once

#include "GameData.hpp"

namespace Client {
	void Connect(std::wstring, std::wstring, std::wstring, std::wstring, std::optional<std::wstring> = {});
	void SendCheck(int64_t);
	void Say(std::string);
	void OnTick();
	void SetZoneData(GameData::Map);
	void CompleteGame();
	void SendDeathLink();
	void Disconnect();
	bool IsMissingLocation(int64_t);
	std::vector<std::wstring> GetHintText(GameData::MajorKeyInfo);
	void CreateMajorKeyHints(GameData::MajorKeyInfo);
}