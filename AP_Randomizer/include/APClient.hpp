#pragma once
#include "DynamicOutput/DynamicOutput.hpp"
#include "Archipelago.h"
#include "APGameManager.hpp"
#include "APCollectible.hpp"
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


	void ClearItems();
	void ReceiveItem(int64_t, bool);
	void CheckLocation(int64_t);
	bool ConnectionStatusChanged();
	void SetSlotNumber(int);
	int connection_timer;
	AP_ConnectionStatus connection_status;
	int slot_number;
}