#pragma once
#include <vector>
#include "GameData.hpp"
#include "Unreal/UObject.hpp"
#include "Unreal/AActor.hpp"
#include "Unreal/World.hpp"

using namespace RC::Unreal;

namespace Engine {
	void ExecuteInGameThread(std::function<void(UObject*)>);
	void ExecuteBlueprintFunction(std::wstring, std::wstring, void*);
	void OnTick(UObject*);
	void SyncItems();
	void SpawnCollectibles();
	UWorld* GetWorld();
}