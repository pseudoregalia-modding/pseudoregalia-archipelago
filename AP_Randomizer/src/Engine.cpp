#pragma once
#include <iostream>
#include "Engine.hpp"
#include "GameData.hpp"
#include "Unreal/AActor.hpp"
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

	std::vector<BlueprintFunctionInfo> blueprint_function_queue;
	std::vector<std::function<void (UObject*)>> function_queue;

	UWorld* Engine::GetWorld() {
		UObject* player_controller = UObjectGlobals::FindFirstOf(STR("PlayerController"));
		// TODO: check whether this actually has to be cast to an AActor anyway
		return static_cast<AActor*>(player_controller)->GetWorld();
	}

	void Engine::ExecuteInGameThread(std::function<void(UObject*)> function) {
		Logger::Log(L"ExecuteInGameThread was called. This function is not currently fully implemented.", Logger::LogType::Warning);
		function_queue.push_back(function);
	}

	void Engine::ExecuteBlueprintFunction(std::wstring new_parent, std::wstring new_name, void* params) {
		Logger::Log(L"Queueing " + new_parent + L"::" + new_name);
		blueprint_function_queue.push_back(BlueprintFunctionInfo(new_parent, new_name, params));
	}

	void Engine::OnTick(UObject* blueprint) {
		// Often (but not always) when I connect, I see an invalid blueprint function go through here.
		// It passes the validation for parent but not for function, and if i try to delete its params or continue instead of return the whole queue breaks.
		// By returning I would expect it to stay in the queue for the next tick, but it doesn't.
		// The log message prints nothing for its parent_name or function_name, but comparing either of them to L"" or .empty() finds nothing.
		// I have no idea where it's coming from or what it is. I fear for my life.

		for (BlueprintFunctionInfo& info : blueprint_function_queue) {
			UObject* parent = UObjectGlobals::FindFirstOf(info.parent_name);
			if (!parent) {
				Logger::Log(L"Could not find blueprint with name" + info.parent_name, Logger::LogType::Error);
				return;
			}
			UFunction* function = parent->GetFunctionByName(info.function_name.c_str());
			if (!function) {
				Logger::Log(L"Could not find function with name" + info.function_name + L" in " + info.parent_name, Logger::LogType::Error);
				return;
			}
			Logger::Log(L"Executing " + info.parent_name + L"::" + info.function_name);
			parent->ProcessEvent(function, info.params);
			delete info.params;
		}
		blueprint_function_queue.clear();

		return;
		for (auto& function : function_queue) {
			function(blueprint);
		}
		function_queue.clear();
	}

	void Engine::SpawnCollectibles() {
		struct CollectibleSpawnInfo {
			int64_t id;
			FVector position;
		};
		std::vector<GameData::Collectible> collectible_vector = GameData::GetCollectiblesOfZone(GetWorld()->GetName());
		for (GameData::Collectible collectible : collectible_vector) {
			if (!collectible.IsChecked()) {
				Logger::Log(L"Spawning collectible with id " + std::to_wstring(collectible.GetID()));
				void* collectible_info = new CollectibleSpawnInfo{ collectible.GetID(), collectible.GetPosition() };
				ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SpawnCollectible", collectible_info);
			}
			else {
				Logger::Log(L"Collectible with id " + std::to_wstring(collectible.GetID()) + L" has already been checked");
			}
		}
	}

	void Engine::SyncItems() {
		SyncHealthPieces();
		SyncSmallKeys();
		SyncMajorKeys();
		SyncAbilities();
	}

	void Engine::SyncAbilities() {
		struct AddUpgradeInfo {
			FName name;
			int count;
		};
		for (auto const& pair : GameData::GetUpgradeTable()) {
			std::unique_ptr<FName> new_name(new FName(pair.first));
			void* upgrade_params = new AddUpgradeInfo{ *new_name, pair.second };
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_AddUpgrade", upgrade_params);
		}
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
		// TODO: reconfigure major keys to be an int and reconfigure this function
		struct MajorKeyInfo {
			int index;
			bool to_give;
		};
		bool* major_keys = GameData::GetMajorKeys();
		for (int i = 0; i < 5; i++)
		{
			void* major_key_params = new MajorKeyInfo{ i, major_keys[i] };
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetMajorKey", major_key_params);
		}
	}

	void Engine::ReceiveItem(int64_t id) {
		GameData::ItemType type = GameData::ReceiveItem(id);
		switch (type)
		{
		case GameData::ItemType::Ability:
			SyncAbilities();
			break;
		case GameData::ItemType::HealthPiece:
			SyncHealthPieces();
			break;
		case GameData::ItemType::SmallKey:
			SyncSmallKeys();
			break;
		case GameData::ItemType::MajorKey:
			SyncMajorKeys();
			break;
		case GameData::ItemType::Unknown:
			break;
		default:
			break;
		}
	}
}