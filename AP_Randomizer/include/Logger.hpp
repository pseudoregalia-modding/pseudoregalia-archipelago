#pragma once
#include <string>

namespace Logger {
	enum class LogType {
		Default,
		Popup,
		System,
		Warning,
		Error
	};

	void Log(std::wstring, LogType = LogType::Default);
	void Log(std::string, LogType = LogType::Default);
	void OnTick();
	void ToggleMessageMute();
	void ToggleMessageHide();
}