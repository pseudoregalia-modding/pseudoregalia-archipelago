#pragma once
#include "APGameManager.hpp"
#include "APClient.hpp"

namespace Pseudoregalia_AP {
	bool APGameManager::hooked_into_returncheck;

	struct CollectibleSpawnInfo {
		int64_t id;
		FVector position;
	};
	
	UWorld* APGameManager::GetWorld() {
		UObject* player_controller = UObjectGlobals::FindFirstOf(STR("PlayerController"));
		return static_cast<AActor*>(player_controller)->GetWorld();
	}

	void APGameManager::OnBeginPlay(AActor* actor) {
		if (actor->GetName().starts_with(STR("BP_APRandomizerInstance"))) {
			UFunction* spawn_function = actor->GetFunctionByName(STR("AP_SpawnCollectible"));
			OnMapLoad(actor, GetWorld());
		}

		if (!hooked_into_returncheck
			&& actor->GetName().starts_with(STR("BP_APCollectible"))) {
				RegisterReturnCheckHook(actor);
				hooked_into_returncheck = true;
		}
	}

	void APGameManager::OnMapLoad(AActor* randomizer_blueprint, UWorld* world) {
		std::vector<APCollectible> collectible_vector = APClient::GetCurrentZoneCollectibles(world->GetName());
		UFunction* spawn_function = randomizer_blueprint->GetFunctionByName(STR("AP_SpawnCollectible"));

		for (APCollectible collectible : collectible_vector) {
			if (collectible.IsChecked()) {
				Output::send<LogLevel::Warning>(STR("Collectible with ID {} has already been sent"), collectible.GetID());
				continue;
			}
			Output::send<LogLevel::Verbose>(STR("Spawned collectible with ID {}"), collectible.GetID());

			CollectibleSpawnInfo new_info = {
				collectible.GetID(),
				collectible.GetPosition(),
			};
			randomizer_blueprint->ProcessEvent(spawn_function, &new_info);
		}
		// Resync items on map load so that players don't lose items after dying or resetting   
		APClient::QueueItemUpdate();
	}

	void APGameManager::OnReturnCheck(Unreal::UnrealScriptFunctionCallableContext& context, void* customdata) {
		struct return_check_params {
			int64_t id;
		};
		auto& params = context.GetParams<return_check_params>();
		Output::send<LogLevel::Verbose>(STR("Obtained check with ID {}\n"), params.id);

		UWorld* world = APGameManager::GetWorld();
		APClient::SendCheck(params.id, world->GetName());
	}

	void APGameManager::RegisterReturnCheckHook(AActor* collectible) {
		UFunction* return_check_function = collectible->GetFunctionByName(STR("ReturnCheck"));
		Unreal::UObjectGlobals::RegisterHook(return_check_function, EmptyFunction, OnReturnCheck, nullptr);
	}

	void APGameManager::EmptyFunction(Unreal::UnrealScriptFunctionCallableContext& context, void* customdata) {
		// exists to avoid crashing upon registering hooks
	}
}