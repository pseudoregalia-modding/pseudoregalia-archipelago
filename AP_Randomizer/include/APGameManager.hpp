#pragma once
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UFunction.hpp>
#include "APCollectible.hpp"

using namespace RC;
using namespace RC::Unreal;

namespace Pseudoregalia_AP {
	class APGameManager {
	public:
		static UWorld* GetWorld();
		static void OnBeginPlay(AActor*);
	private:
		static bool hooked_into_returncheck;
		static void OnReturnCheck(Unreal::UnrealScriptFunctionCallableContext&, void*);
		static void RegisterReturnCheckHook(AActor*);
		static void EmptyFunction(Unreal::UnrealScriptFunctionCallableContext&, void*);
	};
}