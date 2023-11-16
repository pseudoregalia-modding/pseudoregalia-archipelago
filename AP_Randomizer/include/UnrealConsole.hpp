#pragma once
#include "Unreal/UnrealCoreStructs.hpp"
#include "Unreal/FText.hpp"

namespace UnrealConsole {
	using RC::Unreal::TCHAR;

	void ProcessCommand(const TCHAR*);
	void ProcessInput(RC::Unreal::FText);
}