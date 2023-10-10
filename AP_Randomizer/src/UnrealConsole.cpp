#pragma once
#include "UnrealConsole.hpp"
#include <print>
#include <string>

namespace UnrealConsole {
	const char DELIM = ' ';
	void ParseConnect(std::string);
	void ParseMessageOption(std::string);
	std::string GetNextToken(std::string&);
	std::string ConvertTcharToString(const Unreal::TCHAR*);

	void UnrealConsole::ProcessCommand(const Unreal::TCHAR* new_command) {
		std::string command = ConvertTcharToString(new_command);
		std::string first_word = command.substr(0, command.find(DELIM));

		transform(
			first_word.begin(), first_word.end(),
			first_word.begin(),
			tolower);

		if (first_word == "connect") {
			if (command.find(DELIM) == std::string::npos) {
				GameManager::QueueMessage("Please provide an ip address, slot name, and (if necessary) password.");
				return;
			}
			command.erase(0, command.find(DELIM) + 1);
			std::cout << command << "\n";
			UnrealConsole::ParseConnect(command);
		}

		if (first_word == "message" || first_word == "messages") {
			if (command.find(DELIM) == std::string::npos) {
				GameManager::QueueMessage("Please input an option, such as \"mute\" or \"hide\".");
				return;
			}
			command.erase(0, command.find(DELIM) + 1);
			std::cout << command << "\n";
			UnrealConsole::ParseMessageOption(GetNextToken(command));
		}
	}

	std::string UnrealConsole::ConvertTcharToString(const Unreal::TCHAR* tchars) {
		char* new_chars = new char[wcslen(tchars)];
		std::wcstombs(new_chars, tchars, wcslen(tchars));
		new_chars[wcslen(tchars)] = 0;
		std::string new_string = new_chars;
		return new_string;
	}

	void UnrealConsole::ParseConnect(std::string args) {
		std::string ip = GetNextToken(args);
		if (ip.empty()) {
			GameManager::QueueMessage("Please provide an ip address, slot name, and (if necessary) password.");
			return;
		}
		std::cout << "ip:" << ip << "\n";

		std::string slot_name = GetNextToken(args);
		if (slot_name.empty()) {
			GameManager::QueueMessage("Please provide a slot name and (if necessary) password.");
			return;
		}
		std::cout << "slot name:" << slot_name << "\n";

		std::string password = GetNextToken(args);
		std::cout << "password:" << password << "\n";

		Client::Connect(ip.c_str(), slot_name.c_str(), password.c_str());
	}

	void UnrealConsole::ParseMessageOption(std::string option) {
		if (option.empty()) {
			GameManager::QueueMessage("Please input an option, such as \"mute\" or \"hide\".");
			return;
		}

		if (option == "hide" || option == "unhide" || option == "show") {
			GameManager::ToggleMessageHide();
		}

		if (option == "mute" || option == "unmute") {
			GameManager::ToggleMessageMute();
		}
	}

	std::string UnrealConsole::GetNextToken(std::string& input) {
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