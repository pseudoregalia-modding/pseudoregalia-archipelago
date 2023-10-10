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
		static void QueueMessage(std::string);
		static void SetClientConnected(bool);
		static void OnUpdate();
		static void ToggleMessageHide();
		static void ToggleMessageMute();
	private:
		static bool hooked_into_returncheck;
		static bool client_connected;
		static bool messages_hidden;
		static bool messages_muted;
		static std::list<std::string> messages_to_print;
		static std::list<std::string> mini_messages_to_print;
		static int message_timer;
		static void PrintToPlayer(UObject*, std::string);
		static void MiniPrintToPlayer(UObject*, std::string);
		static void OnReturnCheck(Unreal::UnrealScriptFunctionCallableContext&, void*);
		static void EmptyFunction(Unreal::UnrealScriptFunctionCallableContext&, void*);
	};
}