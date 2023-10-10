#pragma once
#include "Engine.hpp"

using namespace RC::Unreal;

namespace Engine {
	struct MajorKeyInfo {
		int index;
		bool to_give;
	};

	struct BlueprintFunctionInfo {
		std::wstring parent_name;
		std::wstring function_name;
		std::shared_ptr<void> params;
	};

	std::vector<BlueprintFunctionInfo> blueprint_function_queue;
	std::vector<std::function<void (UObject*)>> function_queue;

	UWorld* Engine::GetWorld() {
		UObject* player_controller = UObjectGlobals::FindFirstOf(STR("PlayerController"));
		return static_cast<AActor*>(player_controller)->GetWorld();
	}

	void Engine::ExecuteInGameThread(std::function<void(UObject*)> function) {
		function_queue.push_back(function);
	}

	void Engine::ExecuteBlueprintFunction(std::wstring new_parent, std::wstring new_name, std::shared_ptr<void> function) {
		blueprint_function_queue.push_back(BlueprintFunctionInfo(new_parent, new_name, function));
	}

	void Engine::OnTick(UObject* blueprint) {
		for (BlueprintFunctionInfo& info : blueprint_function_queue) {
			UObject* parent = UObjectGlobals::FindFirstOf(info.parent_name);
			UFunction* function = parent->GetFunctionByName(info.function_name.c_str());
			void* ptr = &info.params;
			parent->ProcessEvent(function, ptr);
		}

		for (auto& function : function_queue) {
			function(blueprint);
		}
		function_queue.clear();
	}

	void Engine::SpawnCollectibles() {
		auto spawn_collectibles = [](UObject* blueprint) {
			// Get collectible vector for just the current map
			std::vector<GameData::Collectible> collectible_vector = GameData::GetCollectiblesOfZone(GetWorld()->GetName());
			UFunction* spawn_function = blueprint->GetFunctionByName(STR("AP_SpawnCollectible"));

			struct CollectibleSpawnInfo {
				int64_t id;
				FVector position;
			};
			for (GameData::Collectible collectible : collectible_vector) {
				CollectibleSpawnInfo new_info = {
					collectible.GetID(),
					collectible.GetPosition(),
				};
				if (!collectible.IsChecked()) {
					blueprint->ProcessEvent(spawn_function, &new_info);
				}
			}
			};

		ExecuteInGameThread(spawn_collectibles);
	}

	void Engine::SyncItems() {
		// TODO: considering modularizing this function in-line a bit
		auto item_sync = [](UObject* blueprint) {
			UFunction* set_hp = blueprint->GetFunctionByName(STR("AP_SetHealthPieces"));
			int hp_count = GameData::GetHealthPieces();
			blueprint->ProcessEvent(set_hp, &hp_count);

			// TODO: reconfigure major keys to be an int and reconfigure this function
			UFunction* set_major_keys = blueprint->GetFunctionByName(STR("AP_SetMajorKey"));
			bool* major_keys = GameData::GetMajorKeys();
			for (int i = 0; i < 5; i++)
			{
				MajorKeyInfo params{
					i,
					major_keys[i],
				};
				blueprint->ProcessEvent(set_major_keys, &params);
			}

			UFunction* set_small_keys = blueprint->GetFunctionByName(STR("AP_SetSmallKeys"));
			int small_key_count = GameData::GetSmallKeys();
			blueprint->ProcessEvent(set_small_keys, &small_key_count);


			struct AddUpgradeInfo {
				FName name;
				int count;
			};
			UFunction* add_upgrades = blueprint->GetFunctionByName(STR("AP_AddUpgrade"));
			for (auto const& pair : GameData::GetUpgradeTable()) {
				FName new_name = *new FName(pair.first);

				AddUpgradeInfo params = {
					new_name,
					pair.second,
				};
				blueprint->ProcessEvent(add_upgrades, &params);
			}
			};

		ExecuteInGameThread(item_sync);
	}
}