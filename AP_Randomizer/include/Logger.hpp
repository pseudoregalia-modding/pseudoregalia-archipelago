#pragma once
#include <string>

namespace Logger {
	enum class LogType {
		Default,
		Popup,
		Console,
		System,
		Warning,
		Error
	};

	void Log(std::wstring, LogType = LogType::Default);
	void Log(std::string, LogType = LogType::Default);
	void OnTick();
	void ToggleMessageMute();
	void ToggleMessageHide();
	void PrintToConsole(std::string, std::string);
	void PrintToConsole(std::string);
}

// Don't want to have to prefix Logger twice every time we log something
using Logger::Log;
using Logger::LogType;