#pragma once
#include "APGameManager.hpp"

namespace Pseudoregalia_AP {
	UWorld* APGameManager::GetWorld() {
		UObject* player_controller = UObjectGlobals::FindFirstOf(STR("PlayerController"));
		return static_cast<AActor*>(player_controller)->GetWorld();
	}

	void APGameManager::OnBeginPlay(AActor* actor) {

		if (actor->GetName().starts_with(STR("BP_APRandomizerInstance"))) {
			UFunction* spawn_function = actor->GetFunctionByName(STR("AP_SpawnCollectible"));
			// APClient::OnMapLoad(actor, spawn_function, GetWorld()->GetName());
		}

		if (actor->GetName().starts_with(STR("BP_APCollectible"))) {
			// Hook into ReturnCheck (that's it! honestly this isn't necessary, just put it in eventprocess!)
		}
	}
}