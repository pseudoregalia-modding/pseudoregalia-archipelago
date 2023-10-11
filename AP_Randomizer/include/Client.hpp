#pragma once
#include "Archipelago.h"
#include "GameData.hpp"
#include "Engine.hpp"
#include "Logger.hpp"

#include <locale>
#include <codecvt>
#include <string>

namespace Client {

	void Connect(const char*, const char*, const char*);
	void SendCheck(int64_t, std::wstring);
	void PollServer();
	void CompleteGame();
}