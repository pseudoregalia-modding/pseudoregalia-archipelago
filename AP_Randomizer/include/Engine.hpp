#pragma once
#include <variant>
#include "Unreal/UObject.hpp"

namespace Engine {
	using RC::Unreal::UObject;

	void ExecuteBlueprintFunction(std::variant<std::wstring, UObject*>, std::wstring, std::shared_ptr<void>);
	void OnTick(UObject*);
	void SyncItems();
	void SpawnCollectibles();
	std::wstring GetWorldName();
	void ToggleSlideJump();
	void VaporizeGoat();
}