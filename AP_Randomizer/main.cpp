#define NOMINMAX
#pragma once

#include <Windows.h>
#include "Mod/CppUserModBase.hpp"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/Hooks.hpp"
#include "Unreal/UFunction.hpp"
#include "Unreal/AActor.hpp"
#include "Unreal/FText.hpp"
#include "Unreal/UClass.hpp"
#include "NameTypes.hpp"
#include "clipboardxx.hpp"
#include "Client.hpp"
#include "UnrealConsole.hpp"
#include "Engine.hpp"
#include "Logger.hpp"
#include "Timer.hpp"

class AP_Randomizer : public RC::CppUserModBase {
public:
    struct BoundKey {
        int key;
        std::function<void()> callback;
        bool isPressed = false;
    };
    bool returncheck_hooked = false;
    bool toggleslidejump_hooked = false;
    bool deathlink_hooked = false;
    bool copytext_hooked = false;

    AP_Randomizer() : CppUserModBase() {
        ModName = STR("AP_Randomizer");
        ModVersion = STR("0.1.0");
        ModDescription = STR("archipelago randomizer for pseudoregalia");
        ModAuthors = STR("littlemeowmeow0134");
        //ModIntendedSDKVersion = STR("2.6");
    }

    ~AP_Randomizer()
    {
    }

    auto on_unreal_init() -> void override {
        using namespace RC::Unreal;

        // We need to force blueprint mods to load as soon as possible.
        // Sending an Insert keypress once UE initializes will tell UE4SS' BPModLoaderMod to load them manually.
        {
            INPUT inputs[1] = {};
            inputs[0].type = INPUT_KEYBOARD;
            inputs[0].ki.wVk = VK_INSERT;
            SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
        }

        // I want to make this an AActorTickCallback hook so I can only check actor name,
        // but for some reason that doesn't seem to respond.
        Hook::RegisterProcessEventPreCallback([&](UObject* object, UFunction* function, void* params) {
            static FName randomizer_instance = FName(STR("BP_APRandomizerInstance_C"), RC::Unreal::FNAME_Add);
            static FName receive_tick = FName(STR("ReceiveTick"), RC::Unreal::FNAME_Add);

            bool is_main_randomizer_blueprint = object->GetClassPrivate()->GetNamePrivate() == randomizer_instance;
            bool is_event_tick = function->GetNamePrivate() == receive_tick;

            if (is_main_randomizer_blueprint && is_event_tick) {
                float* delta_seconds = static_cast<float*>(params);
                Timer::OnTick(*delta_seconds);
                Engine::OnTick(object);
            }
            });

        Hook::RegisterProcessConsoleExecCallback([&](UObject* object, const Unreal::TCHAR* command, FOutputDevice& Ar, UObject* executor) -> bool {
            if (command[0] == '/' || command[0] == '!') {
                command++; // Exclude the first character from the array
                UnrealConsole::ProcessCommand(command);
                return true;
            }
            return PropogateCommand(command);
            });

        Hook::RegisterBeginPlayPostCallback([&](AActor* actor) {
            // TODO: Consider moving some of this function out of main
            auto returncheck = [](UnrealScriptFunctionCallableContext& context, void* customdata) {
                Client::SendCheck(context.GetParams<int64_t>());
                };
            auto toggleslidejump = [](UnrealScriptFunctionCallableContext& context, void* customdata) {
                Engine::ToggleSlideJump();
                };
            auto deathlink = [](UnrealScriptFunctionCallableContext& context, void* customdata) {
                Client::SendDeathLink();
                };

            if (!returncheck_hooked
                && actor->GetName().starts_with(STR("BP_APCollectible"))) {

                UFunction* return_check_function = actor->GetFunctionByName(STR("ReturnCheck"));
                if (!return_check_function) {
                    Log(L"Could not find function \"ReturnCheck\" in BP_APCollectible.", LogType::Error);
                    return;
                }
                else {
                    Log(L"Establishing hook for ReturnCheck.");
                }
                Unreal::UObjectGlobals::RegisterHook(return_check_function, EmptyFunction, returncheck, nullptr);
                returncheck_hooked = true;
            }

            if (actor->GetName().starts_with(STR("BP_APRandomizerInstance"))) {
                if (!toggleslidejump_hooked) {
                    UFunction* toggle_function = actor->GetFunctionByName(L"AP_ToggleSlideJump");
                    if (!toggle_function) {
                        Log(L"Could not find function \"AP_ToggleSlideJump\" in BP_APRandomizerInstance.", LogType::Error);
                        return;
                    }
                    else {
                        Log(L"Establishing hook for AP_ToggleSlideJump.");
                    }
                    Unreal::UObjectGlobals::RegisterHook(toggle_function, EmptyFunction, toggleslidejump, nullptr);
                    toggleslidejump_hooked = true;
                }
                // TODO: see if i can make this work in a way that doesn't suck
                // Log(L"Loaded scene " + Engine::GetCurrentMap());
                if (Engine::GetCurrentMap() == GameData::Map::EndScreen) {
                    Client::CompleteGame();
                }
                Engine::SpawnCollectibles();
                Engine::SyncItems();
            }

            if (actor->GetName().starts_with(L"BP_PlayerGoatMain")) {
                if (!deathlink_hooked) {
                    UFunction* death_function = actor->GetFunctionByName(L"BPI_CombatDeath");
                    if (!death_function) {
                        Log(L"Could not find function \"BPI_CombatDeath\" in BP_PlayerGoatMain.", LogType::Error);
                        return;
                    }
                    else {
                        Log(L"Establishing hook for BPI_CombatDeath.");
                    }
                    Unreal::UObjectGlobals::RegisterHook(death_function, EmptyFunction, deathlink, nullptr);
                }
            }
            });

        Hook::RegisterStaticConstructObjectPostCallback([&](const FStaticConstructObjectParameters& params, UObject* object) -> UObject* {
            auto copytext = [&](UnrealScriptFunctionCallableContext& context, void* customdata) {
                // Copies text in highlighted message to clipboard.
                std::wstring wide(context.GetParams<FText>().ToString());

                // TODO: converting to narrow string here breaks nonlatin characters.
                // ClipboardXX only handles narrow chars so I'll need to either fork it or use a different library.
                std::string narrow;
                std::transform(wide.begin(), wide.end(), std::back_inserter(narrow), [](wchar_t c) {
                    return (char)c;
                    });

                clipboardxx::clipboard clipboard;
                clipboard << narrow;
                Logger::Log("copied text to clipboard: " + narrow);
                };

            if (!copytext_hooked
                && object->GetName().starts_with(STR("AP_DeluxeConsole"))) {
                UFunction* copy_function = object->GetFunctionByName(STR("AP_CopyToClipboard"));
                if (!copy_function) {
                    // For some reason this always fails once so don't bother displaying an error.
                    Logger::Log(L"Could not find function \"AP_CopyToClipboard\" in AP_DeluxeConsole.");
                    return object;
                }
                else {
                    Logger::Log(L"Registering hook for AP_CopyToClipboard.");
                }
                Unreal::UObjectGlobals::RegisterHook(copy_function, copytext, EmptyFunction, nullptr);
                copytext_hooked = true;
            }
            return object;
            });

        setup_keybinds();
    }

    bool PropogateCommand(const Unreal::TCHAR* command) {
        // Some console commands do dramatic things like crash on command or simulating debug scenarios.
        // Only "reconnect" and "disconnect" are forbidden from propogating for now since they're easy to mistakenly enter.
        // I haven't decided yet whether more obscure commands should be eaten here as well.
        if (*command == *STR("disconnect") || *command == *STR("reconnect")) {
            return true;
        }
        return false;
    }

    auto on_update() -> void override
    {
        Client::PollServer();
        Logger::OnTick();
        for (auto& boundKey : m_boundKeys)
        {
            if ((GetKeyState(boundKey.key) & 0x8000) && !boundKey.isPressed)
            {
                boundKey.isPressed = true;
            }
            if (!(GetKeyState(boundKey.key) & 0x8000) && boundKey.isPressed)
            {
                boundKey.isPressed = false;
                boundKey.callback();
            }
        }
    }

    auto bind_key(const int& keyCode, const std::function<void()>& callback) -> void {
        BoundKey newBoundKey{
            .key = keyCode,
            .callback = callback,
            .isPressed = false
        };
        m_boundKeys.push_back(newBoundKey);
    }

    auto setup_keybinds() -> void
    {
        // List of key codes at https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
        bind_key(VK_NUMPAD1, [&]() {
            // Client::CompleteGame();
            // Client::Connect("localhost:38281", "goat", "");
            });

        bind_key(VK_NUMPAD2, [&]() {
            });

        bind_key(VK_NUMPAD7, [&]() {
            });

        bind_key(VK_NUMPAD8, [&]() {
            });
    }

    static void EmptyFunction(RC::Unreal::UnrealScriptFunctionCallableContext& context, void* customdata) {
        // Empty function to provide to RegisterHook.
    }

private:
    std::vector<BoundKey> m_boundKeys;
    std::unordered_set<int> m_pressedKeys;
};

#define AP_RANDOMIZER_API __declspec(dllexport)
extern "C"
{
    AP_RANDOMIZER_API RC::CppUserModBase* start_mod()
    {
        return new AP_Randomizer();
    }

    AP_RANDOMIZER_API void uninstall_mod(RC::CppUserModBase* mod)
    {
        delete mod;
    }
}