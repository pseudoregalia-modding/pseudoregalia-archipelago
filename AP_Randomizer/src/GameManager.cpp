#pragma once
#include "GameManager.hpp"
#include <string>
#include <iostream>
#include "Engine.hpp"

namespace GameManager {
	bool client_connected;

	void GameManager::SetClientConnected(bool connected) {
		client_connected = connected;
	}

	void GameManager::OnBeginPlay(AActor* actor) {
		if (actor->GetName().starts_with(STR("BP_APRandomizerInstance"))) {
			if (Engine::GetWorld()->GetName() == STR("EndScreen")) {
				Client::CompleteGame();
			}
			UFunction* spawn_function = actor->GetFunctionByName(STR("AP_SpawnCollectible"));
			Engine::SpawnCollectibles();
			Engine::SyncItems();
		}
	}

	void GameManager::EmptyFunction(Unreal::UnrealScriptFunctionCallableContext& context, void* customdata) {
		// exists to avoid crashing upon registering hooks
	}
}