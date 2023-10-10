#pragma once
#include <vector>
#include "GameData.hpp"
#include "Unreal/UObject.hpp"
#include "Unreal/AActor.hpp"
#include "Unreal/World.hpp"

using namespace RC::Unreal;

namespace Pseudoregalia_AP {
	class Engine {
	public:
		static void ExecuteInGameThread(void (*)(UObject*));
		static void OnTick(UObject*);
		static void SyncItems();
		static void SpawnCollectibles();
		static UWorld* GetWorld();
	private:
		static std::vector<void (*)(UObject*)> function_queue;
	};
}