#pragma once
#include <vector>
#include "Unreal/UObject.hpp"
#include "Unreal/World.hpp"

namespace Engine {
	using namespace RC::Unreal;

	void ExecuteInGameThread(std::function<void(UObject*)>);
	void ExecuteBlueprintFunction(std::wstring, std::wstring, void*);
	void OnTick(UObject*);
	void SyncItems();
	void SpawnCollectibles();
	UWorld* GetWorld();
	void ReceiveItem(int64_t);
}