#pragma once
#include "DynamicOutput/DynamicOutput.hpp"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UFunction.hpp"
#include "Unreal/World.hpp"
#include "APClient.hpp"
#include "APCollectible.hpp"

using namespace RC;
using namespace RC::Unreal;

namespace GameManager {
	UWorld* GetWorld();
	void OnBeginPlay(AActor*);
	void PreProcessEvent(UObject*, UFunction*, void*);
	void QueueMessage(std::string);
	void SetClientConnected(bool);
	void OnUpdate();
	void ToggleMessageHide();
	void ToggleMessageMute();
	void OnReturnCheck(Unreal::UnrealScriptFunctionCallableContext&, void*);
	void EmptyFunction(Unreal::UnrealScriptFunctionCallableContext&, void*);


	bool client_connected;
	bool messages_hidden;
	bool messages_muted;
	std::list<std::string> messages_to_print;
	std::list<std::string> mini_messages_to_print;
	int message_timer;
	void PrintToPlayer(UObject*, std::string);
	void MiniPrintToPlayer(UObject*, std::string);
}