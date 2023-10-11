#pragma once
#include "Client.hpp"
#include <iostream>

namespace UnrealConsole {
	using Logger::LogType;
	using RC::Unreal::TCHAR;

	void ProcessCommand(const TCHAR*);
}