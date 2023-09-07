#pragma once
#include "Archipelago.h"
#include "APCollectible.hpp"

namespace Pseudoregalia_AP {
	class APClient {
	public:
		APClient();
		void Connect(const char*, const char*, const char*);
		void SendCheck(int);
		void OnMapLoad();
	private:
		void Initialize();
	};
}