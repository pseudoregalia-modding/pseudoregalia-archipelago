#pragma once
#include "APGameManager.hpp"
#include <string>
#include <iostream>

namespace Pseudoregalia_AP {
	bool APGameManager::hooked_into_returncheck;
	bool APGameManager::item_update_pending;
	bool APGameManager::spawn_update_pending;
	bool APGameManager::client_connected;
	std::list<std::string> APGameManager::messages_to_print;

	struct PrintToPlayerInfo {
		FText text;
	};

	struct CollectibleSpawnInfo {
		int64_t id;
		FVector position;
	};

	struct AddUpgradeInfo {
		FName name;
		int count;
	};

	struct MajorKeyInfo {
		int index;
		bool to_give;
	};

	struct MinorKeyInfo {
		int count;
	};

	struct HealthPieceInfo {
		int count;
	};

	void APGameManager::QueueItemUpdate() {
		item_update_pending = true;
	}

	void APGameManager::QueueSpawnUpdate() {
		spawn_update_pending = true;
	}
	
	void APGameManager::QueueMessage(std::string message) {
		messages_to_print.push_back(message);
	}

	void APGameManager::SetClientConnected(bool connected) {
		client_connected = connected;
	}
	
	UWorld* APGameManager::GetWorld() {
		UObject* player_controller = UObjectGlobals::FindFirstOf(STR("PlayerController"));
		return static_cast<AActor*>(player_controller)->GetWorld();
	}

	void APGameManager::OnBeginPlay(AActor* actor) {
		if (actor->GetName().starts_with(STR("BP_APRandomizerInstance"))) {
			if (GetWorld()->GetName() == STR("EndScreen")) {
				APClient::CompleteGame();
			}
			UFunction* spawn_function = actor->GetFunctionByName(STR("AP_SpawnCollectible"));
			QueueSpawnUpdate();
			QueueItemUpdate();
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

	void APGameManager::PreProcessEvent(UObject* object, UFunction* function, void* params) {
		// A lot of stuff has to be run in the game thread, so this function handles that.
		// It might be a good idea to just change this to a callback hooked into the main randomizer blueprint's EventTick.

		if (!object->GetName().starts_with(STR("BP_APRandomizerInstance"))) {
			return;
		}

		if (!messages_to_print.empty()) {
			std::string mew = messages_to_print.front();
			messages_to_print.pop_front();
			PrintToPlayer(object, mew);
		}

		if (!client_connected) {
			return;
		}
		if (item_update_pending) {
			item_update_pending = false;
			UFunction* add_upgrade_function = object->GetFunctionByName(STR("AP_AddUpgrade"));
			SyncItems(object, add_upgrade_function);
		}
		if (spawn_update_pending) {
			spawn_update_pending = false;
			SpawnCollectibles(object, GetWorld());
		}
	}

	void APGameManager::SyncItems(UObject* randomizer_blueprint, UFunction* add_upgrade_function) {
		int healthpiece_count = APClient::GetHealthPieces();
		UFunction* set_hp = randomizer_blueprint->GetFunctionByName(STR("AP_SetHealthPieces"));
		{
			HealthPieceInfo params{
				healthpiece_count,
			};
			randomizer_blueprint->ProcessEvent(set_hp, &params);
		}

		bool* major_key_table = APClient::GetMajorKeys();
		UFunction* set_major_keys = randomizer_blueprint->GetFunctionByName(STR("AP_SetMajorKey"));
		for (int i = 0; i < 5; i++)
		{
			MajorKeyInfo params{
				i,
				major_key_table[i],
			};
			randomizer_blueprint->ProcessEvent(set_major_keys, &params);
		}

		int key_count = APClient::GetSmallKeys();
		UFunction* set_small_keys = randomizer_blueprint->GetFunctionByName(STR("AP_SetSmallKeys"));
		{
			MinorKeyInfo params{
				key_count,
			};
			randomizer_blueprint->ProcessEvent(set_small_keys, &params);
		}

		std::map<std::wstring, int> upgrade_table = APClient::GetUpgradeTable();
		for (auto const& pair : upgrade_table) {
			FName new_name = *new FName(pair.first);

			AddUpgradeInfo params = {
				new_name,
				pair.second,
			};
			// Output::send<LogLevel::Verbose>(STR("Attempting to add {} with value {}...\n"), pair.first, pair.second);
			randomizer_blueprint->ProcessEvent(add_upgrade_function, &params);
		}
	}

	void APGameManager::SpawnCollectibles(UObject* randomizer_blueprint, UWorld* world) {
		std::vector<APCollectible> collectible_vector = APClient::GetCurrentZoneCollectibles(world->GetName());
		UFunction* spawn_function = randomizer_blueprint->GetFunctionByName(STR("AP_SpawnCollectible"));

		for (APCollectible collectible : collectible_vector) {
			if (collectible.IsChecked()) {
				Output::send<LogLevel::Warning>(STR("Collectible with ID {} has already been sent\n"), collectible.GetID());
				continue;
			}
			Output::send<LogLevel::Verbose>(STR("Spawned collectible with ID {}\n"), collectible.GetID());

			CollectibleSpawnInfo new_info = {
				collectible.GetID(),
				collectible.GetPosition(),
			};
			randomizer_blueprint->ProcessEvent(spawn_function, &new_info);
		}
	}

	void APGameManager::PrintToPlayer(UObject* randomizer_blueprint, std::string new_message) {
		UFunction* text_func = randomizer_blueprint->GetFunctionByName(STR("AP_QueueMessage"));
		if (!text_func) {
			Output::send<LogLevel::Error>(STR("Error: No function AP_QueueMessage() found in randomizer blueprint. Message could not be printed.\n"));
			return;
		}

		// Need to convert from string to wstring, then wstring to FText
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		FText new_text = *new FText(converter.from_bytes(new_message));
		PrintToPlayerInfo input{
			new_text,
		};
		randomizer_blueprint->ProcessEvent(text_func, &input);
	}

	void APGameManager::RegisterReturnCheckHook(AActor* collectible) {
		UFunction* return_check_function = collectible->GetFunctionByName(STR("ReturnCheck"));
		Unreal::UObjectGlobals::RegisterHook(return_check_function, EmptyFunction, OnReturnCheck, nullptr);
	}

	void APGameManager::EmptyFunction(Unreal::UnrealScriptFunctionCallableContext& context, void* customdata) {
		// exists to avoid crashing upon registering hooks
	}
}