#pragma once
#include <vector>
#include "boost/algorithm/string.hpp"
#include "UnrealConsole.hpp"
#include "Client.hpp"
#include "Logger.hpp"

namespace UnrealConsole {
	using std::string;
	using std::wstring;
	using std::vector;
	using RC::Unreal::FText;

	// Private members
	namespace {
		void ParseConnect(string);
		void ParseMessageOption(string);
		void TryConnect(vector<wstring>);
		vector<wstring> Tokenize(wstring);
		string GetNextToken(string&);
		string ConvertTcharToString(const TCHAR*);

		const char DELIM = ' ';
	} // End private members

	// wstring hashes to run a switch statement on the first word of the command.
	namespace Hashes {
		// std::hash isn't constexpr so we need to implement our own hash function.
		// I just copy/pasted this from https://stackoverflow.com/a/48896410
		constexpr size_t HashWstring(const wstring& to_hash) {
			// Require size_t to be 64-bit.
			static_assert(sizeof(size_t) == 8);

			// FNV-1a 64 bit algorithm
			size_t result = 0xcbf29ce484222325; // FNV offset basis
			for (wchar_t c : to_hash) {
				result ^= c;
				result *= 1099511628211; // FNV prime
			}
			return result;
		}

		constexpr size_t connect = HashWstring(L"connect");
		constexpr size_t disconnect = HashWstring(L"disconnect");
		constexpr size_t hint = HashWstring(L"hint");
	}

	void UnrealConsole::ProcessInput(FText input) {
		wstring command = input.ToString();
		Log(L"AP console command: " + command);
		if (command[0] == *L"/" || command.front() == *L"!") {
			command.erase(0, 1);
			UnrealConsole::ProcessCommand(command);
		}
		else {
			// send say
		}
		return;
	}

	void UnrealConsole::ProcessCommand(wstring input) {
		vector<wstring> args = Tokenize(input);

		// There's no need to check whether the vector is empty,
		// since messages containing only whitespace are filtered out by the blueprint and never sent.
		size_t hashed_command = Hashes::HashWstring(args[0]);
		switch (hashed_command) {
		case Hashes::connect:
			TryConnect(args);
			break;
		case Hashes::disconnect:
			// disconnect
			break;
		case Hashes::hint:
			// send hint
			break;
		default:
			Log(L"Command not recognized: " + input);
			break;
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
		void TryConnect(vector<wstring> args) {
			if (args.size() <= 1) { // !connect
				Log(L"Please provide an ip address, slot name, and (if necessary) password.", LogType::System);
				return;
			}
			if (args.size() <= 2) { // !connect ip
				Log(L"Please provide a slot name and (if necessary) password.", LogType::System);
				return;
			}
			vector<wstring>::iterator iter;
			wstring uri = args[1];
			wstring slot_name = args[2];
			wstring password = L"";
			if (args.size() >= 4) { // !connect ip name password
				password = args[3];
			}

			Log(uri);
			Log(slot_name);
			Log(password);
		}

		vector<wstring> Tokenize(wstring input) {
			// We already need boost for apclientpp so might as well make use of it.
			std::vector<std::wstring> args;
			boost::split(args, input, boost::is_any_of("\t "));

			// Remove extra whitespace from the middle of the string.
			vector<wstring>::iterator iter;
			for (iter = args.begin(); iter != args.end();) {
				if (iter->empty()) {
					iter = args.erase(iter);
				}
				else {
					iter++;
				}
			}

			return args;
		}

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