#pragma once
#include "APConsoleManager.hpp"

namespace Pseudoregalia_AP {
	void APConsoleManager::ProcessCommand(const Unreal::TCHAR* new_command) {
		std::string command = ConvertTcharToString(new_command);
	}

	std::string APConsoleManager::ConvertTcharToString(const Unreal::TCHAR* tchars) {
		char* new_chars = new char[wcslen(tchars)];
		std::wcstombs(new_chars, tchars, wcslen(tchars));
		std::string new_string = new_chars;
		return new_string;
	}
}