#pragma once
#include <vector>
#include "GameData.hpp"

namespace Pseudoregalia_AP {
	class Engine {
	public:
		static void ExecuteInGameThread(void (*)(UObject*));
		static void OnTick(UObject*);
		static void SyncItems();
	private:
		static std::vector<void (*)(UObject*)> function_queue;
	};
}