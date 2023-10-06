#pragma once
#include "DynamicOutput/DynamicOutput.hpp"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UFunction.hpp"
#include "Unreal/World.hpp"
#include "APClient.hpp"
#include "APCollectible.hpp"

using namespace RC;
using namespace RC::Unreal;

namespace Pseudoregalia_AP {
	class APGameManager {
	public:
		static UWorld* GetWorld();
		static void OnBeginPlay(AActor*);
		static void PreProcessEvent(UObject*, UFunction*, void*);
		static void QueueItemUpdate();
		static void QueueSpawnUpdate();
		static void QueueMessage(std::string);
		static void SetClientConnected(bool);
		static void OnUpdate();
		static void ToggleMessageHide();
	private:
		static bool hooked_into_returncheck;
		static bool item_update_pending;
		static bool spawn_update_pending;
		static bool client_connected;
		static bool messages_hidden;
		static std::list<std::string> messages_to_print;
		static std::list<std::string> mini_messages_to_print;
		static int message_timer;
		static void SpawnCollectibles(UObject*, UWorld*);
		static void PrintToPlayer(UObject*, std::string);
		static void MiniPrintToPlayer(UObject*, std::string);
		static void OnReturnCheck(Unreal::UnrealScriptFunctionCallableContext&, void*);
		static void EmptyFunction(Unreal::UnrealScriptFunctionCallableContext&, void*);
		static void SyncItems(UObject*, UFunction*);
	};
}