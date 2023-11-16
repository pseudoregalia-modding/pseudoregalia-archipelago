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
	using std::variant;
	using std::wstring;
	using std::to_wstring;
	using std::shared_ptr;
	using std::mutex;
	using std::lock_guard;
	using std::get;

	// Private members
	namespace {
		void SyncMajorKeys();
		void SyncHealthPieces();
		void SyncSmallKeys();
		void SyncAbilities();

		struct BlueprintFunctionInfo {
			variant<wstring, UObject*> parent;
			wstring function_name;
			shared_ptr<void> params;
		};

		mutex blueprint_function_mutex;
		bool awaiting_item_sync;
		std::queue<BlueprintFunctionInfo> blueprint_function_queue;
	} // End private members


	// Returns the name of the current map.
	wstring Engine::GetWorldName() {
		UObject* player_controller = UObjectGlobals::FindFirstOf(STR("PlayerController"));
		return player_controller->GetWorld()->GetName();
	}

	// Queues up a blueprint function to be executed.
	void Engine::ExecuteBlueprintFunction(variant<wstring, UObject*> new_parent, wstring new_name, shared_ptr<void> params) {
		lock_guard<mutex> guard(blueprint_function_mutex);
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
		lock_guard<mutex> guard(blueprint_function_mutex);
		while (!blueprint_function_queue.empty()) {
			BlueprintFunctionInfo info = blueprint_function_queue.front();
			UObject* object;
			if (std::holds_alternative<wstring>(info.parent)) {
				wstring parent_name = get<wstring>(info.parent);
				object = UObjectGlobals::FindFirstOf(parent_name);
				if (!object) {
					Log(L"Could not find blueprint with name " + parent_name, LogType::Error);
					blueprint_function_queue.pop();
					continue;
				}
			}
			else {
				object = get<UObject*>(info.parent);
				if (object->IsUnreachable()) {
					Log(L"Could not call " + info.function_name + L" because the blueprint was unreachable.", LogType::Error);
					blueprint_function_queue.pop();
					continue;
				}
			}

			UFunction* function = object->GetFunctionByName(info.function_name.c_str());
			if (!function) {
				Log(L"Could not find function " + info.function_name, LogType::Error);
				blueprint_function_queue.pop();
				continue;
			}
			Log(L"Executing " + info.function_name);
			// Need to cast to raw pointer to feed to ProcessEvent, but the memory will still be freed automatically
			void* ptr(info.params.get());
			object->ProcessEvent(function, ptr);
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
			if (!collectible.CanCreate(GameData::GetOptions())) {
				Log(L"Collectible with id " + to_wstring(id) + L" was not spawned because its required options were not met.");
				continue;
			}
			if (collectible.IsChecked()) {
				Log(L"Collectible with id " + to_wstring(id) + L" has already been checked");
				continue;
			}
			Log(L"Spawning collectible with id " + to_wstring(id));
			shared_ptr<void> collectible_info(new CollectibleSpawnInfo{ id, collectible.GetPosition() });
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
		shared_ptr<void> dissolve_delay(new double(0));
		ExecuteBlueprintFunction(L"BP_PlayerGoatMain_C", L"BPI_CombatDeath", dissolve_delay);
	}

	void Engine::DespawnCollectible(const int64_t id) {
		std::vector<UObject*> collectibles{};
		UObjectGlobals::FindAllOf(STR("BP_APCollectible_C"), collectibles);
		for (auto const collectible : collectibles) {
			void* property_ptr = collectible->GetValuePtrByPropertyName(STR("id"));
			int64_t* new_id = static_cast<int64_t*>(property_ptr);
			if (*new_id == id) {
				Log(L"Manually despawning collectible with id " + to_wstring(id));
				ExecuteBlueprintFunction(collectible, L"Despawn", nullptr);
				break;
			}
			// It's fine if we don't find the collectible, it could just be in another map or already despawned
		}
	}


	// Private functions
	namespace {
		void SyncHealthPieces() {
			shared_ptr<void> hp_params(new int(GameData::GetHealthPieces()));
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetHealthPieces", hp_params);
		}

		void SyncSmallKeys() {
			shared_ptr<void> small_key_params(new int(GameData::GetSmallKeys()));
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
			shared_ptr<void> major_key_params(new MajorKeyInfo{ ue_keys });
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
			shared_ptr<void> upgrade_params(new AddUpgradeInfo{ ue_names, ue_counts, toggle });
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetUpgrades", upgrade_params);
		}
	} // End private functions
}