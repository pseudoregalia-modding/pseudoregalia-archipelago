#pragma once
#include "Engine.hpp"
#include <iostream>

namespace Logger {
	void PrintToPlayer(std::string);
	void OnTick();
	void QueueMessage(std::string);
	void ToggleMessageMute();
	void ToggleMessageHide();
}