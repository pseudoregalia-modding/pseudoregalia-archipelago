#pragma once
#include "Logger.hpp"

namespace Logger {
	std::list<std::wstring> message_queue;
	std::list<std::wstring> system_message_queue;
	int message_timer;
	bool messages_hidden;
	bool messages_muted;

	void PrintToPlayer(std::wstring message) {
		if (!messages_hidden) {
			message_queue.push_back(message);
			//std::cout << "pushed " << message << " to queue\n";
		}
	}

	void PrintToPlayer(std::string message) {
		// Convert message to wstring and just pass it to the wstring version of PrintToPlayer
		// Consider logging a warning since this is a bit more roundabout than usually necessary
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		PrintToPlayer(converter.from_bytes(message));
	}

	void Logger::Log(std::string text, LogType type) {
		// Convert message to wstring and just pass it to the wstring version of Log
		// Consider logging a warning since this is a bit more roundabout than usually necessary
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		Log(converter.from_bytes(text), type);
	}

	void Logger::Log(std::wstring text, LogType type = LogType::Default) {
		switch (type) {
		case LogType::Popup:
			send<LogLevel::Verbose>(L"[APRandomizer] Message: " + text + L"\n");
			PrintToPlayer(text);
			break;

		case LogType::System:
			send<LogLevel::Verbose>(L"[APRandomizer] System: " + text + L"\n");
			// There's not really any point in having a system message queue if it doesn't use delays.
			// This should probably just call ExecuteBlueprintFunction directly.
			system_message_queue.push_back(text);
			break;

		case LogType::Warning:
			send<LogLevel::Warning>(L"[APRandomizer] WARNING: " + text + L"\n");
			break;

		case LogType::Error:
			send<LogLevel::Error>(L"[APRandomizer] ERROR: " + text + L"\n");
			// TODO: functionality to display errors to player
			break;

		default:
			send<LogLevel::Default>(L"[APRandomizer] : " + text + L"\n");
			break;
		}
	}

	void Logger::OnTick() {
		// TODO: make a struct or class with a MessageType enum and switch on that to determine how to handle each message
		if (!system_message_queue.empty()) {
			struct PrintToPlayerInfo {
				FText text;
				bool mute_sound;
			};
			std::unique_ptr<FText> new_text(new FText(system_message_queue.front()));
			system_message_queue.pop_front();
			void* params = new PrintToPlayerInfo{ *new_text, messages_muted };
			Engine::ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_PrintMiniMessage", params);
		}

		if (message_timer > 0) {
			message_timer--;
			return;
		}

		if (!message_queue.empty() and message_timer <= 0) {
			struct PrintToPlayerInfo {
				FText text;
				bool mute_sound;
			};
			std::unique_ptr<FText> new_text(new FText(message_queue.front()));
			message_queue.pop_front();
			void* params = new PrintToPlayerInfo{ *new_text, messages_muted };
			Engine::ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_PrintMessage", params);
			message_timer = 600;
		}
	}

	void Logger::ToggleMessageMute() {
		if (messages_muted) {
			messages_muted = false;
			Log(L"Message sounds are no longer muted.", LogType::System);
		}
		else {
			messages_muted = true;
			Log(L"Message sounds are now muted.", LogType::System);
		}
	}

	void Logger::ToggleMessageHide() {
		if (messages_hidden) {
			messages_hidden = false;
			Log(L"Messages are no longer hidden.", LogType::System);
		}
		else {
			messages_hidden = true;
			message_queue.clear();
			Log(L"Messages are now hidden.", LogType::System);
		}
	}
}