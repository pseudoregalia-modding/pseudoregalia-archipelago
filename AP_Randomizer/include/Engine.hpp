#pragma once
#include <vector>
#include "GameData.hpp"
#include "Unreal/UObject.hpp"
#include "Unreal/AActor.hpp"
#include "Unreal/World.hpp"

using namespace RC::Unreal;

namespace Engine {
	void ExecuteInGameThread(void (*)(UObject*));
	void OnTick(UObject*);
	void SyncItems();
	void SpawnCollectibles();
	UWorld* GetWorld();
	

	std::vector<void (*)(UObject*)> function_queue;
}