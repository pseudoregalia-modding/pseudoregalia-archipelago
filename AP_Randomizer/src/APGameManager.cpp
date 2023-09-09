#pragma once
#include "APGameManager.hpp"

namespace Pseudoregalia_AP {
	UWorld* APGameManager::GetWorld() {
		UObject* player_controller = UObjectGlobals::FindFirstOf(STR("PlayerController"));
		return static_cast<AActor*>(player_controller)->GetWorld();
	}
}