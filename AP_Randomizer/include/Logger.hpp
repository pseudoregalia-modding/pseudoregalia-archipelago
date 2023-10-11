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

	void PrintToPlayer(std::wstring);
	void PrintToPlayer(std::string);
	void Log(LogType type, std::wstring text);
	void Log(std::wstring text);
	void OnTick();
	void ToggleMessageMute();
	void ToggleMessageHide();
}