#pragma once
#include "APGameManager.hpp"
#include "APClient.hpp"

namespace Pseudoregalia_AP {
	bool APGameManager::hooked_into_returncheck;

	UWorld* APGameManager::GetWorld() {
		UObject* player_controller = UObjectGlobals::FindFirstOf(STR("PlayerController"));
		return static_cast<AActor*>(player_controller)->GetWorld();
	}

	void APGameManager::OnBeginPlay(AActor* actor) {
		if (actor->GetName().starts_with(STR("BP_APRandomizerInstance"))) {
			UFunction* spawn_function = actor->GetFunctionByName(STR("AP_SpawnCollectible"));
			APClient::OnMapLoad(actor, spawn_function, GetWorld()->GetName());
		}

		if (!hooked_into_returncheck
			&& actor->GetName().starts_with(STR("BP_APCollectible"))) {
				RegisterReturnCheckHook(actor);
				hooked_into_returncheck = true;
		}
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