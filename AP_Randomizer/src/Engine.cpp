#pragma once
#include "Engine.hpp"

using namespace RC::Unreal;

namespace Engine {
	struct MajorKeyInfo {
		int index;
		bool to_give;
	};

	std::vector<void (*)(UObject*)> function_queue;

	UWorld* Engine::GetWorld() {
		UObject* player_controller = UObjectGlobals::FindFirstOf(STR("PlayerController"));
		return static_cast<AActor*>(player_controller)->GetWorld();
	}

	void Engine::ExecuteInGameThread(void (*function)(UObject*) ) {
		function_queue.push_back(function);
	}

	void Engine::OnTick(UObject* blueprint) {
		for (auto& function : function_queue) {
			function(blueprint);
		}
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