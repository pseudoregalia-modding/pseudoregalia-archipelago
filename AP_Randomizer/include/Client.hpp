#pragma once
#include "DynamicOutput/DynamicOutput.hpp"
#include "Archipelago.h"
#include "GameManager.hpp"
#include "GameData.hpp"
#include "Engine.hpp"

#include <locale>
#include <codecvt>
#include <string>

namespace Client {
	void Connect(const char*, const char*, const char*);
	void SendCheck(int64_t, std::wstring);
	void PollServer();
	void CompleteGame();
}