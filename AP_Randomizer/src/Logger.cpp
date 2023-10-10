#pragma once
#include "Logger.hpp"

namespace Logger {
	std::list<std::string> message_queue;
	std::list<std::string> system_message_queue;
	int message_timer;
	bool messages_hidden;
	bool messages_muted;

	void Logger::ToggleMessageMute() {
		if (messages_muted) {
			messages_muted = false;
			system_message_queue.push_back("Message sounds are no longer muted.");
		}
		else {
			messages_muted = true;
			system_message_queue.push_back("Message sounds are now muted.");
		}
	}

	void Logger::ToggleMessageHide() {
		if (messages_hidden) {
			messages_hidden = false;
			system_message_queue.push_back("Messages are no longer hidden.");
		}
		else {
			messages_hidden = true;
			message_queue.clear();
			system_message_queue.push_back("Messages are now hidden.");
		}
	}

	void Logger::OnTick() {
		// TODO: make a struct or class with a MessageType enum and switch on that to determine how to handle each message
		if (!system_message_queue.empty()) {
			std::string next_message = system_message_queue.front();
			system_message_queue.pop_front();

			struct PrintToPlayerInfo {
				FText text;
				bool mute_sound;
			};
			// Need to convert from string to wstring, then wstring to FText
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::unique_ptr<FText> new_text(new FText(converter.from_bytes(next_message)));
			void* params = new PrintToPlayerInfo{ *new_text, messages_muted };
			Engine::ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_PrintMiniMessage", params);
		}

		if (message_timer > 0) {
			message_timer--;
			return;
		}

		if (!message_queue.empty() and message_timer <= 0) {
			std::string next_message = message_queue.front();
			message_queue.pop_front();

			// Need to convert from string to wstring, then wstring to FText
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::unique_ptr<FText> new_text(new FText(converter.from_bytes(next_message)));
			struct PrintToPlayerInfo {
				FText text;
				bool mute_sound;
			};
			void* params = new PrintToPlayerInfo{ *new_text, messages_muted };
			Engine::ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_PrintMessage", params);
			message_timer = 2000;
		}
	}

	void Logger::PrintToPlayer(std::string message) {
		if (!messages_hidden) {
			message_queue.push_back(message);
			std::cout << "pushed " << message << " to queue\n";
		}
	}
}