#pragma once

#include "Unreal/Core/HAL/Platform.hpp"
#include "Unreal/UnrealCoreStructs.hpp"
#include "Unreal/FText.hpp"

namespace UnrealConsole {
	using RC::Unreal::TCHAR;

	void ProcessCommand(std::wstring);
	void ProcessInput(RC::Unreal::FText);
}