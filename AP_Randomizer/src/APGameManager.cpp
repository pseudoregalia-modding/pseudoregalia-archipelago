#pragma once
#include "APGameManager.hpp"
#include <string>
#include <iostream>
#include "Engine.hpp"

namespace GameManager {
	struct PrintToPlayerInfo {
		FText text;
		bool mute_sound;
	};

	bool client_connected;
	bool messages_hidden;
	bool messages_muted;
	std::list<std::string> messages_to_print;
	std::list<std::string> mini_messages_to_print;
	int message_timer;
	void PrintToPlayer(UObject*, std::string);
	void MiniPrintToPlayer(UObject*, std::string);

	void GameManager::QueueMessage(std::string message) {
		if (messages_hidden) {
			return;
		}
		messages_to_print.push_back(message);
	}

	void GameManager::SetClientConnected(bool connected) {
		client_connected = connected;
	}
	
	UWorld* GameManager::GetWorld() {
		UObject* player_controller = UObjectGlobals::FindFirstOf(STR("PlayerController"));
		return static_cast<AActor*>(player_controller)->GetWorld();
	}

	void GameManager::OnUpdate() {
		if (message_timer > 0) {
			message_timer--;
		}
	}

	void GameManager::ToggleMessageMute() {
		if (messages_muted) {
			messages_muted = false;
			mini_messages_to_print.push_back("Message sounds are no longer muted.");
			Output::send<LogLevel::Verbose>(STR("Message sounds are no longer muted."));
		}
		else {
			messages_muted = true;
			mini_messages_to_print.push_back("Message sounds are now muted.");
			Output::send<LogLevel::Verbose>(STR("Message sounds are now muted."));
		}
	}

	void GameManager::ToggleMessageHide() {
		if (messages_hidden) {
			messages_hidden = false;
			mini_messages_to_print.push_back("Messages are no longer hidden.");
			Output::send<LogLevel::Verbose>(STR("Messages are no longer hidden."));
		}
		else {
			messages_hidden = true;
			messages_to_print.clear();
			mini_messages_to_print.push_back("Messages are now hidden.");
			Output::send<LogLevel::Verbose>(STR("Messages are now hidden."));
		}
	}

	void GameManager::OnBeginPlay(AActor* actor) {
		if (actor->GetName().starts_with(STR("BP_APRandomizerInstance"))) {
			if (GetWorld()->GetName() == STR("EndScreen")) {
				Client::CompleteGame();
			}
			UFunction* spawn_function = actor->GetFunctionByName(STR("AP_SpawnCollectible"));
			Engine::SpawnCollectibles();
			Engine::SyncItems();
		}
	}

	void GameManager::PreProcessEvent(UObject* object, UFunction* function, void* params) {
		// A lot of stuff has to be run in the game thread, so this function handles that.
		// It might be a good idea to just change this to a callback hooked into the main randomizer blueprint's EventTick,
		// but I haven't yet sorted out how to pass around a pointer to the blueprint by doing that.
		// It might honestly be a good idea to just shovel everything into one great big OnEventTick function and define everything it does inline.

		if (!object->GetName().starts_with(STR("BP_APRandomizerInstance"))) {
			return;
		}

		if (!messages_to_print.empty() and message_timer <= 0 and function->GetName() == STR("ReceiveTick")) {
			std::string mew = messages_to_print.front();
			messages_to_print.pop_front();
			PrintToPlayer(object, mew);
			message_timer = 600;
		}

		if (!mini_messages_to_print.empty()) {
			std::string mew = mini_messages_to_print.front();
			mini_messages_to_print.pop_front();
			MiniPrintToPlayer(object, mew);
		}

		if (!client_connected) {
			return;
		}
	}

	void GameManager::PrintToPlayer(UObject* randomizer_blueprint, std::string new_message) {
		UFunction* text_func = randomizer_blueprint->GetFunctionByName(STR("AP_PrintMessage"));
		if (!text_func) {
			Output::send<LogLevel::Error>(STR("Error: No function AP_PrintMessage found in randomizer blueprint.\n"));
			return;
		}

		// Need to convert from string to wstring, then wstring to FText
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		FText new_text = *new FText(converter.from_bytes(new_message));
		PrintToPlayerInfo input{
			new_text,
			messages_muted,
		};
		randomizer_blueprint->ProcessEvent(text_func, &input);
	}

	void GameManager::MiniPrintToPlayer(UObject* randomizer_blueprint, std::string new_message) {
		UFunction* text_func = randomizer_blueprint->GetFunctionByName(STR("AP_PrintMiniMessage"));
		if (!text_func) {
			Output::send<LogLevel::Error>(STR("Error: No function AP_PrintMiniMessage found in randomizer blueprint.\n"));
			return;
		}

		// Need to convert from string to wstring, then wstring to FText
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		FText new_text = *new FText(converter.from_bytes(new_message));
		PrintToPlayerInfo input{
			new_text,
		};
		randomizer_blueprint->ProcessEvent(text_func, &input);
	}

	void GameManager::EmptyFunction(Unreal::UnrealScriptFunctionCallableContext& context, void* customdata) {
		// exists to avoid crashing upon registering hooks
	}
}