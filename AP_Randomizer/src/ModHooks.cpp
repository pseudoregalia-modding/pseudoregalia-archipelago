#pragma once

#include "Unreal/UClass.hpp"
#include "Unreal/FText.hpp"
#include "ModHooks.hpp"
#include "Client.hpp"
#include "Logger.hpp"
#include "Engine.hpp"
#include "UnrealConsole.hpp"
#include "StringOps.hpp"

namespace ModHooks {
	using namespace RC::Unreal;
	using std::function;
	using std::optional;
	using std::unordered_map;
	using std::vector;
	using std::wstring;

	namespace {
		auto nop = [](UnrealScriptFunctionCallableContext& context, void* customdata) {};

		struct ModHook {
			wstring class_name;
			wstring function_name;
			UnrealScriptFunctionCallable pre;
			UnrealScriptFunctionCallable post;
			bool hooked;

			ModHook(wstring new_class_name, wstring new_function_name, UnrealScriptFunctionCallable new_pre = nop,
					UnrealScriptFunctionCallable new_post = nop) {
				class_name = new_class_name;
				function_name = new_function_name;
				pre = new_pre;
				post = new_post;
				hooked = false;
			}
		};

		vector<ModHook> actor_hooks = {
			ModHook(L"BP_APCollectible_C", L"ReturnCheck",
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					Client::SendCheck(context.GetParams<int64_t>());
				}),
			ModHook(L"BP_APRandomizerInstance_C", L"AP_ToggleSlideJump",
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					Engine::ToggleSlideJump();
				}),
			ModHook(L"BP_PlayerGoatMain_C", L"BPI_CombatDeath", nop,
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					Client::SendDeathLink();
				}),
			ModHook(L"BP_TimeTrial_C", L"manageResult", nop,
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					Engine::SpawnTimeTrialCollectibleIfBeaten(context.Context);
				}),
			ModHook(L"BP_NPC_C", L"BPI_EndInteract", nop,
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					GameData::Interact(context.Context->GetName());
				}),
			ModHook(L"BP_RestChair_C", L"BPI_TryInteract", nop,
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					GameData::Interact(context.Context->GetName());
				}),
			ModHook(L"BP_ExamineTextPopup_C", L"BPI_TryInteract",
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					Engine::SetTombstoneText(context.Context);
				}),
			ModHook(L"BP_ExamineTextPopup_C", L"BPI_EndInteract", nop,
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					GameData::Interact(context.Context->GetName());
					Engine::CreateMajorKeyHints(context.Context);
				}),
			ModHook(L"BP_Note_C", L"BPI_TryInteract", nop,
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					GameData::ReadNote(context.Context->GetName());
				}),
		};

		vector<ModHook> object_hooks = {
			ModHook(L"AP_DeluxeConsole_C", L"AP_CopyToClipboard",
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					StringOps::CopyToClipboard(context.GetParams<FText>().ToString());
				}),
			ModHook(L"AP_DeluxeConsole_C", L"AP_SendMessage",
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					FText input = context.GetParams<FText>();
					UnrealConsole::ProcessInput(input);
				}),
			ModHook(L"UI_Note_C", L"BndEvt__UI_Note_UI_GenericButton_K2Node_ComponentBoundEvent_0_CommonButtonBaseClicked__DelegateSignature", nop,
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					GameData::FinishNote();
				}),
			ModHook(L"UI_Note_C", L"BPI_TextAdvance", nop,
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					GameData::FinishNote();
				}),
			ModHook(L"UI_Note_C", L"Close Self", nop,
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					GameData::FinishNote();
				}),
			ModHook(L"AP_NewFileMenu_C", L"Connect", nop,
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					Engine::StartConnectHandshake(context.Context);
					struct ConnectParams {
						FText domain;
						FText port;
						FText slot_name;
						FText password;
					};
					auto& params = context.GetParams<ConnectParams>();
					Client::Connect(params.domain.ToString(), params.port.ToString(), params.slot_name.ToString(),
						            params.password.ToString());
				}),
			ModHook(L"AP_NewFileMenu_C", L"GoBack",
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					Client::Disconnect();
				}),
			ModHook(L"AP_ExistingFileMenu_C", L"Connect", nop,
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					Engine::StartConnectHandshake(context.Context);
					struct ConnectParams {
						FText seed;
						FText domain;
						FText port;
						FText slot_name;
						FText password;
					};
					auto& params = context.GetParams<ConnectParams>();
					Client::Connect(params.domain.ToString(), params.port.ToString(), params.slot_name.ToString(),
									params.password.ToString(), params.seed.ToString());
				}),
			ModHook(L"AP_ExistingFileMenu_C", L"GoBack",
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					Client::Disconnect();
				}),
			ModHook(L"AP_FileSelectMenuButton_C", L"CheckVersionCompatibility",
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					Engine::CheckVersionCompatibility(context);
				}),
			ModHook(L"AP_ExistingFileMenu_C", L"CheckVersionCompatibility",
				[](UnrealScriptFunctionCallableContext& context, void* customdata) {
					Engine::CheckVersionCompatibility(context);
				}),
		};

		typedef function<void(AActor*)> ActorCallback;
		const unordered_map<wstring, ActorCallback> begin_play_post_callbacks = {
			{L"BP_APRandomizerInstance_C", [](AActor* actor) {
				Engine::OnSceneLoad(actor);
			}},
			{L"BP_Note_C", [](AActor* actor) {
				optional<wstring> note_text = GameData::GetNoteText(actor->GetName());
				if (!note_text) return;
				FText* inText = actor->GetValuePtrByPropertyName<FText>(L"inText");
				inText->SetString(FString(note_text->c_str()));
			}},
		};

		typedef function<void(UObject*)> ObjectCallback;
		const unordered_map<wstring, ObjectCallback> static_construct_object_post_callbacks = {
			{L"UI_HUD", [](UObject* object) {
				Engine::ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_CreateConsole", nullptr);
			}},
		};

		void RegisterModHook(UObject*, ModHook&);
	}

	void RegisterActorHooks(AActor* actor) {
		for (auto& hook : actor_hooks) {
			if (hook.hooked) continue;
			if (actor->GetClassPrivate()->GetName() != hook.class_name) continue;
			RegisterModHook(actor, hook);
		}
	}

	void RunBeginPlayPostCallback(AActor* actor) {
		wstring class_name = actor->GetClassPrivate()->GetName();
		if (!begin_play_post_callbacks.contains(class_name)) return;
		const auto& callback = begin_play_post_callbacks.at(class_name);
		callback(actor);
	}

	void RegisterObjectHooks(UObject* object) {
		wstring name = object->GetName();
		for (auto& hook : object_hooks) {
			if (hook.hooked) continue;
			// this function is used primarily to hook into widgets, which are created ad hoc and so their names look
			// like {class_name}_{numbers}. since this function is called in StaticConstructObjectPostCallback, the
			// blueprint itself will also be passed to this function when loaded, and its name looks like {class_name}.
			// tacking on the underscore at the end filters this out.
			if (!name.starts_with(hook.class_name + L"_")) continue;
			RegisterModHook(object, hook);
		}
	}

	void RunStaticConstructObjectPostCallback(UObject* object) {
		wstring name = object->GetName();
		for (const auto& [class_name, callback] : static_construct_object_post_callbacks) {
			if (!name.starts_with(class_name + L"_")) continue;
			callback(object);
			return;
		}
	}


	namespace {
		void RegisterModHook(UObject* object, ModHook& hook) {
			UFunction* func = object->GetFunctionByName(hook.function_name.c_str());
			if (!func) {
				Log(L"Could not find function \"" + hook.function_name + L"\" in " + hook.class_name, LogType::Error);
				return;
			}
			Log(L"Establishing hook for \"" + hook.function_name + L"\" in " + hook.class_name);
			UObjectGlobals::RegisterHook(func, hook.pre, hook.post, nullptr);
			hook.hooked = true;

		}
	}
}
