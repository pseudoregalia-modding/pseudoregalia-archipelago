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
				APGameManager::QueueMessage("Please provide an ip address, slot name, and (if necessary) password.");
				return;
			}
			command.erase(0, command.find(DELIM) + 1);
			std::cout << command << "\n";
			APConsoleManager::ParseConnect(command);
		}

		if (first_word == "message") {
			if (command.find(DELIM) == std::string::npos) {
				APGameManager::QueueMessage("Please input an option, such as \"mute\" or \"hide\".");
				return;
			}
			command.erase(0, command.find(DELIM) + 1);
			std::cout << command << "\n";
			APConsoleManager::ParseMessageOption(GetNextToken(command));
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
		std::string ip = GetNextToken(args);
		if (ip.empty()) {
			APGameManager::QueueMessage("Please provide an ip address, slot name, and (if necessary) password.");
			return;
		}
		std::cout << "ip:" << ip << "\n";

		std::string slot_name = GetNextToken(args);
		if (slot_name.empty()) {
			APGameManager::QueueMessage("Please provide a slot name and (if necessary) password.");
			return;
		}
		std::cout << "slot name:" << slot_name << "\n";

		std::string password = GetNextToken(args);
		std::cout << "password:" << password << "\n";

		APClient::Connect(ip.c_str(), slot_name.c_str(), password.c_str());
	}

	void APConsoleManager::ParseMessageOption(std::string option) {
		if (option.empty()) {
			APGameManager::QueueMessage("Please input an option, such as \"mute\" or \"hide\".");
			return;
		}

		if (option == "hide" || option == "unhide" || option == "show") {
			APGameManager::ToggleMessageHide();
		}

		if (option == "mute" || option == "unmute") {
			APGameManager::ToggleMessageMute();
		}
	}

	std::string APConsoleManager::GetNextToken(std::string& input) {
		while (input[0] == DELIM) {
			input.erase(input.begin());
		}
		std::string token;
		if (input[0] == '"') {
			// look for second double quote and take everything between them as one token
			token = input.substr(1, input.find('"', 1)-1);
			input.erase(0, input.find('"', 1)+1);
			return token;
		}
		token = input.substr(0, input.find(DELIM));
		input.erase(0, input.find(DELIM));
		return token;
	}
}