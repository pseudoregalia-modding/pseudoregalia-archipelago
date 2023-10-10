#pragma once
#include <vector>
#include "GameData.hpp"
#include "Unreal/UObject.hpp"
#include "Unreal/AActor.hpp"

namespace Pseudoregalia_AP {
	class Engine {
	public:
		static void ExecuteInGameThread(void (*)(UObject*));
		static void OnTick(UObject*);
		static void SyncItems();
	private:
		static UWorld* GetWorld();
		static std::vector<void (*)(UObject*)> function_queue;
	};
}