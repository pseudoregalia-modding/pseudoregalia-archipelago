#pragma once
#include "APGameManager.hpp"
#include <string>
#include <iostream>
#include "Engine.hpp"

namespace Pseudoregalia_AP {
	bool APGameManager::hooked_into_returncheck;
	bool APGameManager::client_connected;
	bool APGameManager::messages_hidden;
	bool APGameManager::messages_muted;
	std::list<std::string> APGameManager::messages_to_print;
	std::list<std::string> APGameManager::mini_messages_to_print;
	int APGameManager::message_timer;

	struct PrintToPlayerInfo {
		FText text;
		bool mute_sound;
	};

	struct CollectibleSpawnInfo {
		int64_t id;
		FVector position;
	};

	struct AddUpgradeInfo {
		FName name;
		int count;
	};

	struct MajorKeyInfo {
		int index;
		bool to_give;
	};

	struct MinorKeyInfo {
		int count;
	};

	struct HealthPieceInfo {
		int count;
	};
	
	void APGameManager::QueueMessage(std::string message) {
		if (messages_hidden) {
			return;
		}
		messages_to_print.push_back(message);
	}

	void APGameManager::SetClientConnected(bool connected) {
		client_connected = connected;
	}
	
	UWorld* APGameManager::GetWorld() {
		UObject* player_controller = UObjectGlobals::FindFirstOf(STR("PlayerController"));
		return static_cast<AActor*>(player_controller)->GetWorld();
	}

	void APGameManager::OnUpdate() {
		if (message_timer > 0) {
			message_timer--;
		}
	}

	void APGameManager::ToggleMessageMute() {
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

	void APGameManager::ToggleMessageHide() {
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

	void APGameManager::OnBeginPlay(AActor* actor) {
		if (actor->GetName().starts_with(STR("BP_APRandomizerInstance"))) {
			if (GetWorld()->GetName() == STR("EndScreen")) {
				APClient::CompleteGame();
			}
			UFunction* spawn_function = actor->GetFunctionByName(STR("AP_SpawnCollectible"));
			Engine::SpawnCollectibles();
			Engine::SyncItems();
		}

		if (!hooked_into_returncheck
			&& actor->GetName().starts_with(STR("BP_APCollectible"))) {

				UFunction* return_check_function = actor->GetFunctionByName(STR("ReturnCheck"));
				if (!return_check_function) {
					Output::send<LogLevel::Error>(STR("Could not find function ReturnCheck in BP_APCollectible."));
					return;
				}
				Unreal::UObjectGlobals::RegisterHook(return_check_function, EmptyFunction, OnReturnCheck, nullptr);
				hooked_into_returncheck = true;
		}
	}

	void APGameManager::OnReturnCheck(Unreal::UnrealScriptFunctionCallableContext& context, void* customdata) {
		struct return_check_params {
			int64_t id;
		};
		auto& params = context.GetParams<return_check_params>();
		Output::send<LogLevel::Verbose>(STR("Obtained check with ID {}\n"), params.id);

		UWorld* world = APGameManager::GetWorld();
		APClient::SendCheck(params.id, world->GetName());
	}

	void APGameManager::PreProcessEvent(UObject* object, UFunction* function, void* params) {
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

	void APGameManager::PrintToPlayer(UObject* randomizer_blueprint, std::string new_message) {
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

	void APGameManager::MiniPrintToPlayer(UObject* randomizer_blueprint, std::string new_message) {
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

	void APGameManager::EmptyFunction(Unreal::UnrealScriptFunctionCallableContext& context, void* customdata) {
		// exists to avoid crashing upon registering hooks
	}
}