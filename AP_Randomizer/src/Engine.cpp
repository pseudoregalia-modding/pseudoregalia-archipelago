#pragma once
#include <mutex>
#include <queue>
#include "Unreal/TArray.hpp"
#include "Unreal/World.hpp"
#include "Engine.hpp"
#include "GameData.hpp"
#include "Logger.hpp"

namespace Engine {
	using namespace RC::Unreal; // Give Engine easy access to Unreal objects

	// Private members
	namespace {
		void SyncMajorKeys();
		void SyncHealthPieces();
		void SyncSmallKeys();
		void SyncAbilities();

		struct BlueprintFunctionInfo {
			std::wstring parent_name;
			std::wstring function_name;
			std::shared_ptr<void> params;
		};

		std::mutex blueprint_function_mutex;
		bool awaiting_item_sync;
		std::queue<BlueprintFunctionInfo> blueprint_function_queue;
	} // End private members


	// Returns the name of the current map.
	std::wstring Engine::GetWorldName() {
		UObject* player_controller = UObjectGlobals::FindFirstOf(STR("PlayerController"));
		return player_controller->GetWorld()->GetName();
	}

	// Queues up a blueprint function to be executed.
	void Engine::ExecuteBlueprintFunction(std::wstring new_parent, std::wstring new_name, std::shared_ptr<void> params) {
		std::lock_guard<std::mutex> guard(blueprint_function_mutex);
		blueprint_function_queue.push(BlueprintFunctionInfo(new_parent, new_name, params));
	}

	// Runs once every engine tick.
	void Engine::OnTick(UObject* blueprint) {
		// Queue up item syncs together to avoid queueing a bajillion functions on connection or world release.
		if (awaiting_item_sync) {
			SyncHealthPieces();
			SyncSmallKeys();
			SyncMajorKeys();
			SyncAbilities();
			awaiting_item_sync = false;
		}

		// Engine tick runs in a separate thread from the client so it needs to be locked.
		std::lock_guard<std::mutex> guard(blueprint_function_mutex);
		while (!blueprint_function_queue.empty()) {
			BlueprintFunctionInfo info = blueprint_function_queue.front();
			UObject* parent = UObjectGlobals::FindFirstOf(info.parent_name);
			if (!parent) {
				Logger::Log(L"Could not find blueprint with name " + info.parent_name, Logger::LogType::Error);
				blueprint_function_queue.pop();
				continue;
			}

			UFunction* function = parent->GetFunctionByName(info.function_name.c_str());
			if (!function) {
				Logger::Log(L"Could not find function " + info.function_name, Logger::LogType::Error);
				blueprint_function_queue.pop();
				continue;
			}
			Logger::Log(L"Executing " + info.function_name);
			// Need to cast to raw pointer to feed to ProcessEvent, but the memory will still be freed automatically
			void* ptr(info.params.get());
			parent->ProcessEvent(function, ptr);
			blueprint_function_queue.pop();
		}
	}

	// Calls blueprint's AP_SpawnCollectible function for each unchecked collectible in a map.
	void Engine::SpawnCollectibles() {
		// This function must loop through instead of calling once with an array;
		// as of 10/11/23 the params struct method I use can't easily represent FVectors or FTransforms in C++.
		// This might be worked around by storing positions as three separate numbers instead and constructing the vectors in BP,
		// but I don't think it's worth changing right now since this is just called once each map load.
		struct CollectibleSpawnInfo {
			int64_t new_id;
			FVector position;
		};
		std::unordered_map<int64_t, GameData::Collectible> collectible_map = GameData::GetCollectiblesOfZone(GetWorldName());
		for (const auto& [id, collectible] : collectible_map) {
			// Return if the collectible shouldn't be spawned based on options
			if (!collectible.RequiredOption().empty()
				&& !GameData::GetOption(collectible.RequiredOption())) {
				Logger::Log("Collectible with id " + std::to_string(id) + " was not spawned since option " + collectible.RequiredOption() + " was not enabled.");
				continue;
			}

			if (collectible.IsChecked()) {
				Logger::Log(L"Collectible with id " + std::to_wstring(id) + L" has already been checked");
				continue;
			}

			Logger::Log(L"Spawning collectible with id " + std::to_wstring(id));
			std::shared_ptr<void> collectible_info(new CollectibleSpawnInfo{ id, collectible.GetPosition() });
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SpawnCollectible", collectible_info);
		}
	}

	// Queues all item sync functions.
	void Engine::SyncItems() {
		awaiting_item_sync = true;
	}

	void Engine::ToggleSlideJump() {
		if (GameData::ToggleSlideJump()) {
			SyncAbilities();
		}
	}

	// Kills Sybil.
	void Engine::VaporizeGoat() {
		std::shared_ptr<void> dissolve_delay(new double(0));
		ExecuteBlueprintFunction(L"BP_PlayerGoatMain_C", L"BPI_CombatDeath", dissolve_delay);
	}


	// Private functions
	namespace {
		void SyncHealthPieces() {
			std::shared_ptr<void> hp_params(new int(GameData::GetHealthPieces()));
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetHealthPieces", hp_params);
		}

		void SyncSmallKeys() {
			std::shared_ptr<void> small_key_params(new int(GameData::GetSmallKeys()));
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetSmallKeys", small_key_params);
		}

		void SyncMajorKeys() {
			struct MajorKeyInfo {
				TArray<bool> keys;
			};
			TArray<bool> ue_keys;
			bool* major_keys = GameData::GetMajorKeys();
			for (int i = 0; i < 5; i++) {
				ue_keys.Add(major_keys[i]);
			}
			std::shared_ptr<void> major_key_params(new MajorKeyInfo{ ue_keys });
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetMajorKeys", major_key_params);
		}

		void SyncAbilities() {
			struct AddUpgradeInfo {
				TArray<FName> names;
				TArray<int> counts;
				bool slidejump_disabled;
			};
			TArray<FName> ue_names;
			TArray<int> ue_counts;
			bool toggle = GameData::SlideJumpDisabled();

			for (const auto& [upgrade_name, upgrade_count] : GameData::GetUpgradeTable()) {
				FName new_name(upgrade_name);
				ue_names.Add(new_name);
				ue_counts.Add(upgrade_count);
			}
			std::shared_ptr<void> upgrade_params(new AddUpgradeInfo{ ue_names, ue_counts, toggle });
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetUpgrades", upgrade_params);
		}
	} // End private functions
}