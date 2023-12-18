#pragma once
#include "Unreal/FText.hpp"
#include "DynamicOutput/DynamicOutput.hpp"
#include "Logger.hpp"
#include "Engine.hpp"
#include "Timer.hpp"
#include "StringOps.hpp"

namespace Logger {
	using namespace RC::Output;
	using namespace RC::LogLevel;
	using RC::Unreal::FText;
	using std::wstring;
	using std::string;

	// Private members
	namespace {
		void PrintToPlayer(wstring);
		void PrintToPlayer(string);

		std::list<wstring> message_queue;
		bool popups_locked;
		// Just over 3 seconds is long enough to ensure only 2 popups can be on screen at once
		const float popup_delay_seconds(3.2f);
		bool messages_hidden;
		bool messages_muted;
	} // End private members


	void Logger::Log(string text, LogType type) {
		Log(StringOps::ToWide(text), type);
	}

	void Logger::Log(wstring text, LogType type) {
		switch (type) {
		case LogType::Popup: {
			send<LogLevel::Verbose>(L"[APRandomizer] Message: " + text + L"\n");
			PrintToPlayer(text);
			break;
		}

		case LogType::Console: {
			send<LogLevel::Default>(L"[APRandomizer] Console: " + text + L"\n");
			break;
		}

		case LogType::System: {
			send<LogLevel::Verbose>(L"[APRandomizer] System: " + text + L"\n");
			wstring markdown_text = L"<System>" + text + L"</>";
			PrintToConsole(markdown_text, text);
			break;
		}

		case LogType::Warning: {
			send<LogLevel::Warning>(L"[APRandomizer] WARNING: " + text + L"\n");
			break;
		}

		case LogType::Error: {
			send<LogLevel::Error>(L"[APRandomizer] ERROR: " + text + L"\n");
			wstring markdown_text = L"<Error>Error: " + text + L"</>";
			PrintToConsole(markdown_text, text);
			break;
		}

		default: {
			send<LogLevel::Default>(L"[APRandomizer] : " + text + L"\n");
			break;
		}
		} // End switch
	}

	void Logger::PrintToConsole(std::wstring markdown_text, std::wstring plain_text) {
		struct ConsoleLineInfo {
			FText markdown;
			FText plain;
		};
		FText ue_markdown(markdown_text);
		FText ue_plain(plain_text);
		std::shared_ptr<void> params(new ConsoleLineInfo{ ue_markdown, ue_plain });
		Engine::ExecuteBlueprintFunction(L"AP_DeluxeConsole_C", L"AP_PrintToConsole", params);
	}

	void Logger::PrintToConsole(std::wstring text) {
		PrintToConsole(text, text);
	}

	void Logger::OnTick() {
		// This implementation is slightly awkward but the whole UI is gonna get refactored eventually anyway so whatever.
		if (popups_locked) {
			return;
		}

		if (!message_queue.empty()) {
			struct PrintToPlayerInfo {
				FText text;
				bool mute_sound;
			};
			FText new_text(message_queue.front());
			message_queue.pop_front();
			std::shared_ptr<void> params(new PrintToPlayerInfo{ new_text, messages_muted });
			Engine::ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_PrintMessage", params);
			Timer::RunTimerInGame(popup_delay_seconds, &popups_locked);
		}
	}

	void Logger::ToggleMessageMute() {
		messages_muted = !messages_muted;
		if (messages_muted) {
			Log(L"Popup sounds are now muted.", LogType::System);
		}
		else {
			Log(L"Popup sounds are no longer muted.", LogType::System);
		}
	}

	void Logger::ToggleMessageHide() {
		messages_hidden = !messages_hidden;
		if (messages_hidden) {
			message_queue.clear();
			Log(L"Popups are now hidden.", LogType::System);
		}
		else {
			Log(L"Popups are no longer hidden.", LogType::System);
		}
	}


	// Private functions
	namespace {
		void PrintToPlayer(wstring message) {
			if (!messages_hidden) {
				message_queue.push_back(message);
			}
		}

		void PrintToPlayer(string message) {
			PrintToPlayer(StringOps::ToWide(message));
		}
	} // End private functions
}