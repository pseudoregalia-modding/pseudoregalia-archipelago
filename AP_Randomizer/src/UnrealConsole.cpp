#pragma once
#include "boost/algorithm/string.hpp"
#include "UnrealConsole.hpp"
#include "Client.hpp"
#include "Logger.hpp"
#include <vector>

namespace UnrealConsole {
	using std::string;
	using std::wstring;
	using std::vector;
	using RC::Unreal::FText;

	// Private members
	namespace {
		void ParseConnect(string);
		void ParseMessageOption(string);
		string GetNextToken(string&);
		string ConvertTcharToString(const TCHAR*);

		const char DELIM = ' ';
	} // End private members

	void UnrealConsole::ProcessInput(FText input) {
		wstring command = input.ToString();
		Log(L"AP console command: " + command);

		// We already need boost for apclientpp so might as well make use of it.
		std::vector<std::wstring> words;
		boost::split(words, command, boost::is_any_of("\t "));

		vector<wstring>::iterator iter;
		for (iter = words.begin(); iter != words.end();) {
			if (iter->empty()) {
				iter = words.erase(iter);
			}
			else {
				iter++;
			}
		}
	}

	void UnrealConsole::ProcessCommand(const TCHAR* new_command) {
		// TODO: This should maybe just return something and have a parent check on its return value to decide what to do, 
		// but for now it's not really worth refactoring
		string command = ConvertTcharToString(new_command);
		string first_word = command.substr(0, command.find(DELIM));
		transform(first_word.begin(), first_word.end(), first_word.begin(), tolower); // Convert the first word in the command to lowercase
		Log("AP console command: " + command);

		if (first_word == "connect") {
			if (command.find(DELIM) == string::npos) {
				Log(L"Please provide an ip address, slot name, and (if necessary) password.", LogType::System);
				return;
			}
			command.erase(0, command.find(DELIM) + 1);
			UnrealConsole::ParseConnect(command);
		}

		if (first_word == "disconnect") {
			Client::Disconnect();
		}

		if (first_word == "message" || first_word == "messages") {
			if (command.find(DELIM) == string::npos) {
				Log(L"Please input an option, such as \"mute\" or \"hide\".", LogType::System);
				return;
			}
			command.erase(0, command.find(DELIM) + 1);
			UnrealConsole::ParseMessageOption(GetNextToken(command));
		}
	}


	// Private functions
	namespace {
		string ConvertTcharToString(const TCHAR* tchars) {
			// Handling strings instead of wstrings here because they need to be narrow eventually to pass to APCpp.
			// There's no character set conversion so if nonlatin unicode characters are entered this will break completely.
			std::wstring wide(tchars);
			string narrow;
			std::transform(wide.begin(), wide.end(), std::back_inserter(narrow), [](wchar_t c) {
				return (char)c;
				});
			return narrow;
		}

		void ParseConnect(string args) {
			string ip = GetNextToken(args);
			if (ip.empty()) {
				Log("Please provide an ip address, slot name, and (if necessary) password.", LogType::System);
				return;
			}

			string slot_name = GetNextToken(args);
			if (slot_name.empty()) {
				Log("Please provide a slot name and (if necessary) password.", LogType::System);
				return;
			}

			string password = GetNextToken(args);
			Client::Connect(ip, slot_name, password);
		}

		void ParseMessageOption(string option) {
			if (option.empty()) {
				Log("Please input an option, such as \"mute\" or \"hide\".", LogType::System);
				return;
			}
			if (option == "hide" || option == "unhide" || option == "show") {
				Logger::ToggleMessageHide();
			}
			if (option == "mute" || option == "unmute") {
				Logger::ToggleMessageMute();
			}
		}

		string GetNextToken(string& input) {
			// Gets the next space-separated word and removes it from the input string.
			while (input[0] == DELIM) {
				input.erase(input.begin());
			}
			string token;
			if (input[0] == '"') {
				// look for second double quote and take everything between them as one token
				token = input.substr(1, input.find('"', 1) - 1);
				input.erase(0, input.find('"', 1) + 1);
				return token;
			}
			token = input.substr(0, input.find(DELIM));
			input.erase(0, input.find(DELIM));
			return token;
		}
	} // End private functions
}