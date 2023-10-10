#pragma once
#include "DynamicOutput/DynamicOutput.hpp"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UFunction.hpp"
#include "Unreal/World.hpp"
#include "Client.hpp"
#include "Collectible.hpp"

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
}