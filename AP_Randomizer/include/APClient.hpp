#pragma once
#include "Archipelago.h"

namespace Pseudoregalia_AP {
	class APClient {
	public:
		APClient();
		void Connect(const char*, const char*, const char*);
		void SendCheck(int);
		void CompleteGame();
	private:
		void Initialize();
		static void ClearItems();
		static void ReceiveItem(int64_t, bool);
		static void CheckLocation(int64_t);
	};
}