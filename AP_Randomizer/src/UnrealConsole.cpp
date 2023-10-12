#pragma once
#include "Unreal/FText.hpp"
#include "UnrealConsole.hpp"
#include "Client.hpp"
#include "Logger.hpp"

namespace UnrealConsole {
	const char DELIM = ' ';
	void ParseConnect(std::string);
	void ParseMessageOption(std::string);
	std::string GetNextToken(std::string&);
	std::string ConvertTcharToString(const TCHAR*);

	void UnrealConsole::ProcessCommand(const TCHAR* new_command) {
		// TODO: This should maybe just return something and have a parent check on its return value to decide what to do, 
		// but for now it's not really worth refactoring
		std::string command = ConvertTcharToString(new_command);
		std::string first_word = command.substr(0, command.find(DELIM));
		transform(first_word.begin(), first_word.end(), first_word.begin(), tolower); // Convert the first word in the command to lowercase
		Logger::Log("AP console command: " + command);

		if (first_word == "connect") {
			if (command.find(DELIM) == std::string::npos) {
				Logger::Log(L"Please provide an ip address, slot name, and (if necessary) password.", Logger::LogType::System);
				return;
			}
			command.erase(0, command.find(DELIM) + 1);
			UnrealConsole::ParseConnect(command);
		}

		if (first_word == "message" || first_word == "messages") {
			if (command.find(DELIM) == std::string::npos) {
				Logger::Log(L"Please input an option, such as \"mute\" or \"hide\".", Logger::LogType::System);
				return;
			}
			command.erase(0, command.find(DELIM) + 1);
			UnrealConsole::ParseMessageOption(GetNextToken(command));
		}
	}

	std::string UnrealConsole::ConvertTcharToString(const TCHAR* tchars) {
		// Handling strings instead of wstrings here because they need to be narrow eventually to pass to APCpp.
		// There's no character set conversion so if nonlatin unicode characters are entered this will break completely.
		std::wstring wide(tchars);
		std::string narrow;
		std::transform(wide.begin(), wide.end(), std::back_inserter(narrow), [](wchar_t c) {
			return (char)c;
			});
		return narrow;
	}

	void UnrealConsole::ParseConnect(std::string args) {
		std::string ip = GetNextToken(args);
		if (ip.empty()) {
			Logger::Log("Please provide an ip address, slot name, and (if necessary) password.", Logger::LogType::System);
			return;
		}

		std::string slot_name = GetNextToken(args);
		if (slot_name.empty()) {
			Logger::Log("Please provide a slot name and (if necessary) password.", Logger::LogType::System);
			return;
		}

		std::string password = GetNextToken(args);
		Client::Connect(ip.c_str(), slot_name.c_str(), password.c_str());
	}

	void UnrealConsole::ParseMessageOption(std::string option) {
		if (option.empty()) {
			Logger::Log("Please input an option, such as \"mute\" or \"hide\".", Logger::LogType::System);
			return;
		}
		if (option == "hide" || option == "unhide" || option == "show") {
			Logger::ToggleMessageHide();
		}
		if (option == "mute" || option == "unmute") {
			Logger::ToggleMessageMute();
		}
	}

	std::string UnrealConsole::GetNextToken(std::string& input) {
		// Gets the next space-separated word and removes it from the input string.
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