#pragma once
#include "MapManager.hpp"
#include "GoatManager.hpp"
#include "Archipelago.h"

namespace Pseudoregalia_AP {
	class APClient {
	public:
		APClient();
		void Connect(const char*, const char*, const char*);
		void SendCheck(int);
		void CompleteGame();
		void OnMapLoad();
	private:
		void Initialize();
		static void ClearItems();
		static void ReceiveItem(int64_t, bool);
		static void CheckLocation(int64_t);
	};
}