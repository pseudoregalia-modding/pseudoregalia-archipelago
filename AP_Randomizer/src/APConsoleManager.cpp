#pragma once
#include "APConsoleManager.hpp"
#include <print>
#include <string>

namespace Pseudoregalia_AP {
	const char APConsoleManager::DELIM = ' ';

	void APConsoleManager::ProcessCommand(const Unreal::TCHAR* new_command) {
		std::string command = ConvertTcharToString(new_command);
		std::string first_word = command.substr(0, command.find(DELIM));

		transform(
			first_word.begin(), first_word.end(),
			first_word.begin(),
			tolower);

		if (first_word == "connect") {
			if (command.find(DELIM) == std::string::npos) {
				std::cout << "Please provide an address.\n";
				return;
			}
			command.erase(0, command.find(DELIM) + 1);
			std::cout << command << "\n";
			APConsoleManager::ParseConnect(command);
		}
	}

	std::string APConsoleManager::ConvertTcharToString(const Unreal::TCHAR* tchars) {
		char* new_chars = new char[wcslen(tchars)];
		std::wcstombs(new_chars, tchars, wcslen(tchars));
		new_chars[wcslen(tchars)] = 0;
		std::string new_string = new_chars;
		return new_string;
	}

	void APConsoleManager::ParseConnect(std::string args) {
		if (args.empty()) {
			std::cout << "Please provide an address.\n";
			return;
		}

		std::string ip = args.substr(0, args.find(DELIM));
	}
}