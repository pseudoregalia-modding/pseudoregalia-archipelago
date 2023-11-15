#pragma once
#include <variant>
#include "Unreal/UObject.hpp"

namespace Engine {
	using RC::Unreal::UObject;

	void ExecuteBlueprintFunction(std::variant<std::wstring, UObject*>, std::wstring, std::shared_ptr<void>);
	void OnTick(UObject*);
	void SyncItems();
	void SpawnCollectibles();
	void DespawnCollectible(const int64_t);
	std::wstring GetWorldName();
	void ToggleSlideJump();
	void VaporizeGoat();
}