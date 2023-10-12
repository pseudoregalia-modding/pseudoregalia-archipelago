#pragma once
#include "Unreal/UObject.hpp"

namespace Engine {
	using namespace RC::Unreal;

	void ExecuteInGameThread(std::function<void(UObject*)>);
	void ExecuteBlueprintFunction(std::wstring, std::wstring, void*);
	void OnTick(UObject*);
	void SyncItems();
	void SpawnCollectibles();
	std::wstring GetWorldName();
}