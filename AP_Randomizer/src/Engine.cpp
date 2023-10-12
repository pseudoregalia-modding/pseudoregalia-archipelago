#pragma once
#include "Unreal/TArray.hpp"
#include "Unreal/World.hpp"
#include "Engine.hpp"
#include "GameData.hpp"
#include "Logger.hpp"

namespace Engine {
	void SyncMajorKeys();
	void SyncHealthPieces();
	void SyncSmallKeys();
	void SyncAbilities();

	struct BlueprintFunctionInfo {
		std::wstring parent_name;
		std::wstring function_name;
		void* params;
	};

	bool awaiting_item_sync;
	std::list<BlueprintFunctionInfo> blueprint_function_queue;
	std::list<std::function<void (UObject*)>> function_queue;

	std::wstring Engine::GetWorldName() {
		UObject* player_controller = UObjectGlobals::FindFirstOf(STR("PlayerController"));
		return player_controller->GetWorld()->GetName();
	}

	void Engine::ExecuteInGameThread(std::function<void(UObject*)> function) {
		Logger::Log(L"ExecuteInGameThread was called. This function is not currently fully implemented.", Logger::LogType::Warning);
		function_queue.push_back(function);
	}

	void Engine::ExecuteBlueprintFunction(std::wstring new_parent, std::wstring new_name, void* params) {
		blueprint_function_queue.push_back(BlueprintFunctionInfo(new_parent, new_name, params));
	}

	void Engine::OnTick(UObject* blueprint) {
		if (awaiting_item_sync) {
			SyncHealthPieces();
			SyncSmallKeys();
			SyncMajorKeys();
			SyncAbilities();
			awaiting_item_sync = false;
		}

		for (BlueprintFunctionInfo& info : blueprint_function_queue) {
			// Since I only call functions in BP_APRandomizerInstance right now there's no need to run FindFirstof to find a parent blueprint.
			// If I decide to pull back from using blueprint functions as much (maybe after TMap is implemented in UE4SS)
			// this functionality may become useful again.

			/*UObject* parent = UObjectGlobals::FindFirstOf(info.parent_name);
			if (!parent) {
				Logger::Log(L"Could not find blueprint with name " + info.parent_name, Logger::LogType::Error);
				delete info.params;
				blueprint_function_queue.pop_front();
				continue;
			}*/

			UFunction* function = blueprint->GetFunctionByName(info.function_name.c_str());
			if (!function) {
				Logger::Log(L"Could not find function " + info.function_name, Logger::LogType::Error);
				delete info.params;
				blueprint_function_queue.pop_front();
				continue;
			}
			Logger::Log(L"Executing " + info.function_name);
			blueprint->ProcessEvent(function, info.params);
			delete info.params;
			blueprint_function_queue.pop_front();
		}

		return;
		for (auto& function : function_queue) {
			function(blueprint);
			function_queue.pop_front();
		}
	}

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
		for (auto& pair : collectible_map) {
			int64_t id = pair.first;
			GameData::Collectible collectible = pair.second;

			if (!collectible.IsChecked()) {
				Logger::Log(L"Spawning collectible with id " + std::to_wstring(id));
				void* collectible_info = new CollectibleSpawnInfo{ id, collectible.GetPosition() };
				ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SpawnCollectible", collectible_info);
			}
			else {
				Logger::Log(L"Collectible with id " + std::to_wstring(id) + L" has already been checked");
			}
		}
	}

	void Engine::SyncItems() {
		awaiting_item_sync = true;
	}

	void Engine::SyncAbilities() {
		struct AddUpgradeInfo {
			TArray<FName> names;
			TArray<int> counts;
		};
		TArray<FName> ue_names;
		TArray<int> ue_counts;

		for (const auto& pair : GameData::GetUpgradeTable()) {
			std::unique_ptr<FName> new_name(new FName(pair.first));
			ue_names.Add(*new_name);
			ue_counts.Add(pair.second);
		}

		void* upgrade_params = new AddUpgradeInfo{ ue_names, ue_counts };
		ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetUpgrades", upgrade_params);
	}

	void Engine::SyncHealthPieces() {
		void* hp_params = new int(GameData::GetHealthPieces());
		ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetHealthPieces", hp_params);
	}

	void Engine::SyncSmallKeys() {
		void* small_key_params = new int(GameData::GetSmallKeys());
		ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetSmallKeys", small_key_params);
	}

	void Engine::SyncMajorKeys() {
		struct MajorKeyInfo {
			TArray<bool> keys;
		};
		TArray<bool> ue_keys;
		bool* major_keys = GameData::GetMajorKeys();
		for (int i = 0; i < 5; i++)
		{
			ue_keys.Add(major_keys[i]);
		}

		void* major_key_params = new MajorKeyInfo{ ue_keys };
		ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetMajorKeys", major_key_params);
	}
}