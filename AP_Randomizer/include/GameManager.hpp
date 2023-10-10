#pragma once
#include "DynamicOutput/DynamicOutput.hpp"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UFunction.hpp"
#include "Unreal/World.hpp"
#include "Client.hpp"
#include "Collectible.hpp"
#include "Logger.hpp"

using namespace RC;
using namespace RC::Unreal;

namespace GameManager {
	UWorld* GetWorld();
	void OnBeginPlay(AActor*);
	void PreProcessEvent(UObject*, UFunction*, void*);
	void SetClientConnected(bool);
	void OnReturnCheck(Unreal::UnrealScriptFunctionCallableContext&, void*);
	void EmptyFunction(Unreal::UnrealScriptFunctionCallableContext&, void*);
}