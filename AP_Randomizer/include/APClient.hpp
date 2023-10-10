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

namespace Pseudoregalia_AP {
	class APClient {
	public:
		static void Initialize();
		static void Connect(const char*, const char*, const char*);
		static void SendCheck(int64_t, std::wstring);
		static void PollServer();
		static void CompleteGame();
	private:
		static void ClearItems();
		static void ReceiveItem(int64_t, bool);
		static void CheckLocation(int64_t);
		static void CheckLocation(APCollectible&, std::wstring);
		static bool ConnectionStatusChanged();
		static void SetSlotNumber(int);
		static int connection_timer;
		static AP_ConnectionStatus connection_status;
		static int slot_number;
	};
}