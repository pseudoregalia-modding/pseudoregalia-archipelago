#pragma once
#include <optional>
#include <cwctype>
#include "boost/algorithm/string.hpp"
#include "UnrealConsole.hpp"
#include "Client.hpp"
#include "Logger.hpp"
#include "StringOps.hpp"
#include "Engine.hpp"

namespace UnrealConsole {
	using std::string;
	using std::wstring;
	using RC::Unreal::FText;

	namespace Hashes {
		using StringOps::HashWstring;
		constexpr size_t connect = HashWstring(L"connect");
		constexpr size_t disconnect = HashWstring(L"disconnect");
		constexpr size_t hint = HashWstring(L"hint");
		constexpr size_t hint_location = HashWstring(L"hint_location");
		constexpr size_t release = HashWstring(L"release");
		constexpr size_t collect = HashWstring(L"collect");
		constexpr size_t remaining = HashWstring(L"remaining");
		constexpr size_t missing = HashWstring(L"missing");
		constexpr size_t checked = HashWstring(L"checked");
		constexpr size_t getitem = HashWstring(L"getitem");
		constexpr size_t popups = HashWstring(L"popups");
		constexpr size_t countdown = HashWstring(L"countdown");
		constexpr size_t spawn = HashWstring(L"spawn");
		constexpr size_t breaker = HashWstring(L"breaker");
		constexpr size_t help = HashWstring(L"help");
	}

	// Private members
	namespace {
		void ParseConnect(string);
		void ParseMessageOption(string);
		void TryConnect(wstring);
		string GetNextToken(string&);
		string ConvertTcharToString(const TCHAR*);
		string ConvertWstringToString(wstring);

		const char DELIM = ' ';
	} // End private members

	void UnrealConsole::ProcessInput(FText input) {
		wstring command = input.ToString();
		Log(L"AP console input: " + command);
		if (command[0] == *L"/") {
			command.erase(0, 1);
			UnrealConsole::ProcessCommand(command);
		}
		else {
			Client::Say(StringOps::ToNarrow(command));
		}
		return;
	}

	void UnrealConsole::ProcessCommand(wstring input) {
		wstring command = input.substr(0, input.find(L' '));
		// Convert the command to lowercase to make it case-insensitive.
		std::transform(command.begin(), command.end(), command.begin(), tolower);
		wstring args = L"";
		if (input.find(L' ') != wstring::npos) {
			args = (input.substr(input.find(L' ') + 1));
		}

		size_t hashed_command = Hashes::HashWstring(command);
		switch (hashed_command) {
		case Hashes::connect:
			Engine::PrintToConsole(L"/" + input);
			// TODO double check ap connection?
			Log("You are already connected!", LogType::System);
			break;
		case Hashes::disconnect:
			Engine::PrintToConsole(L"/" + input);
			Log(L"To disconnect from the server, return to the main menu.", LogType::System);
			break;
		case Hashes::popups: {
			Engine::PrintToConsole(L"/" + input);
			wstring popup_args = L"";
			for (const wchar_t c : args) {
				if (c != L' ') {
					popup_args.push_back(c);
				}
			}
			std::transform(popup_args.begin(), popup_args.end(), popup_args.begin(), tolower);

			if (popup_args == L"hide" || popup_args == L"unhide" || popup_args == L"show") {
				Engine::TogglePopupsHide();
			}
			else if (popup_args == L"mute" || popup_args == L"unmute") {
				Engine::TogglePopupsMute();
			}
			else {
				Log(L"Please input either \"/popups mute\" or \"/popups hide\".", LogType::System);
			}
			break;
		}
		case Hashes::spawn:
			Engine::WarpToSpawn();
			break;
		case Hashes::release:
		case Hashes::collect:
		case Hashes::hint:
		case Hashes::hint_location:
		case Hashes::remaining:
		case Hashes::missing:
		case Hashes::checked:
		case Hashes::getitem:
		case Hashes::countdown:
			Engine::PrintToConsole(L"/" + input);
			Log(L"This command is not supported using /. Use ! instead:", LogType::System);
			Log(L"!" + input, LogType::System);
			break;
		case Hashes::breaker:
			Engine::PrintToConsole(L"/" + input);
			Engine::RecallBreaker();
			break;
		case Hashes::help:
			Engine::PrintToConsole(L"/" + input);
			Engine::PrintToConsole(
				L"<System>/spawn</>\n"
				L"    <System>Save game and warp to spawn.</>\n"
				L"<System>/breaker</>\n"
				L"    <System>Recall Dream Breaker. Only works if you have obtained it.</>\n"
				L"<System>/popups [hide|mute]</>\n"
				L"    <System>Hide/show or mute/unmute item and deathlink popups.</>");
			break;
		default:
			Engine::PrintToConsole(L"/" + input);
			Log(L"Command not recognized: " + input, LogType::System);
			Log(L"Known commands: help, spawn, breaker, popups", LogType::System);
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

		// TODO can just we get rid of this whole function?
		/*if (first_word == "connect") {
			if (command.find(DELIM) == string::npos) {
				Log(L"Please provide an ip address, slot name, and (if necessary) password.", LogType::System);
				return;
			}
			command.erase(0, command.find(DELIM) + 1);
			UnrealConsole::ParseConnect(command);
		}*/

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
			string narrow = StringOps::ToNarrow(wide);
			return narrow;
		}

		void ParseMessageOption(string option) {
			if (option.empty()) {
				Log("Please input an option, such as \"mute\" or \"hide\".", LogType::System);
				return;
			}
			if (option == "hide" || option == "unhide" || option == "show") {
				Engine::TogglePopupsHide();
			}
			if (option == "mute" || option == "unmute") {
				Engine::TogglePopupsMute();
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