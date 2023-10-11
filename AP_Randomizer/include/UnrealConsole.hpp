#pragma once
#include "DynamicOutput/DynamicOutput.hpp"
#include "Client.hpp"
#include <iostream>

namespace UnrealConsole {
	using Logger::LogType;
	using RC::Unreal::TCHAR;

	void ProcessCommand(const TCHAR*);
}