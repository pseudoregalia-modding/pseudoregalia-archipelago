#pragma once
#include "Engine.hpp"
#include <iostream>

namespace Logger {
	using RC::Unreal::FText;

	void PrintToPlayer(std::string);
	void OnTick();
	void ToggleMessageMute();
	void ToggleMessageHide();
}