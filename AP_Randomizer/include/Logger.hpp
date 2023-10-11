#pragma once
#include "Engine.hpp"
#include <iostream>

namespace Logger {
	using RC::Unreal::FText;

	enum class LogType {
		Default,
		Popup,
		System,
		Warning,
		Error
	};

	void PrintToPlayer(std::string);
	void OnTick();
	void ToggleMessageMute();
	void ToggleMessageHide();
}