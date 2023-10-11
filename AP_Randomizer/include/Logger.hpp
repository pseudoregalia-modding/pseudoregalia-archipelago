#pragma once
#include "Engine.hpp"
#include "DynamicOutput/DynamicOutput.hpp"

namespace Logger {
	using namespace RC::Output;
	using namespace RC::LogLevel;
	using RC::Unreal::FText;

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