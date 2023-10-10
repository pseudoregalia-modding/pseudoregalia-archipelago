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
	
	UWorld* GameManager::GetWorld() {
		UObject* player_controller = UObjectGlobals::FindFirstOf(STR("PlayerController"));
		return static_cast<AActor*>(player_controller)->GetWorld();
	}

	void GameManager::OnBeginPlay(AActor* actor) {
		if (actor->GetName().starts_with(STR("BP_APRandomizerInstance"))) {
			if (GetWorld()->GetName() == STR("EndScreen")) {
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