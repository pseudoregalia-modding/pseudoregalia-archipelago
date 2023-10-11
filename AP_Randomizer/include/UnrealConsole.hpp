#pragma once
#include "Unreal/UnrealCoreStructs.hpp"

namespace UnrealConsole {
	using Logger::LogType;
	using RC::Unreal::TCHAR;

	void ProcessCommand(const TCHAR*);
}