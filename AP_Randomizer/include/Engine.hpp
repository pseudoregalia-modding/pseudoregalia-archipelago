#pragma once
#include <vector>
#include "GameData.hpp"
#include "Unreal/UObject.hpp"
#include "Unreal/AActor.hpp"
#include "Unreal/World.hpp"
#include "Logger.hpp"

namespace Engine {
	using namespace RC::Unreal;

	void ExecuteInGameThread(std::function<void(UObject*)>);
	void ExecuteBlueprintFunction(std::wstring, std::wstring, void*);
	void OnTick(UObject*);
	void SyncItems();
	void SpawnCollectibles();
	UWorld* GetWorld();
}