#pragma once
#include "Logger.hpp"

namespace Logger {
	std::list<std::wstring> message_queue;
	std::list<std::wstring> system_message_queue;
	int message_timer;
	bool messages_hidden;
	bool messages_muted;

	void Logger::PrintToPlayer(std::wstring message) {
		if (!messages_hidden) {
			message_queue.push_back(message);
			//std::cout << "pushed " << message << " to queue\n";
		}
	}

	void Logger::PrintToPlayer(std::string message) {
		// Convert message to wstring and just pass it to the wstring version of PrintToPlayer
		// Consider logging a warning since this is a bit more roundabout than usually necessary
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		PrintToPlayer(converter.from_bytes(message));
	}

	void Log(std::string text, LogType type) {
		// Convert message to wstring and just pass it to the wstring version of Log
		// Consider logging a warning since this is a bit more roundabout than usually necessary
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		Log(converter.from_bytes(text), type);
	}

	void Log(std::wstring text, LogType type = LogType::Default) {
		switch (type)
		{
		case LogType::Popup:
			send<LogLevel::Verbose>(text);
			break;
		case LogType::System:
			send<LogLevel::Verbose>(text);
			break;
		case LogType::Warning:
			send<LogLevel::Warning>(text);
			break;
		case LogType::Error:
			send<LogLevel::Error>(text);
			break;
		default:
			send<LogLevel::Default>(text);
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
			system_message_queue.push_back(L"Message sounds are no longer muted.");
		}
		else {
			messages_muted = true;
			system_message_queue.push_back(L"Message sounds are now muted.");
		}
	}

	void Logger::ToggleMessageHide() {
		if (messages_hidden) {
			messages_hidden = false;
			system_message_queue.push_back(L"Messages are no longer hidden.");
		}
		else {
			messages_hidden = true;
			message_queue.clear();
			system_message_queue.push_back(L"Messages are now hidden.");
		}
	}
}