#define NOMINMAX
#pragma once

#include <Windows.h>
#include "Mod/CppUserModBase.hpp"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/Hooks.hpp"
#include "Unreal/UFunction.hpp"
#include "Unreal/AActor.hpp"
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

        // I want to make this an AActorTickCallback hook so I can use delta_seconds and only check actor name,
        // but for some reason that doesn't seem to respond.
        // Might check with UE4SS devs on that.
        Hook::RegisterProcessEventPreCallback([&](UObject* object, UFunction* function, void* params) {
            if (object->GetName().starts_with(STR("BP_APRandomizerInstance")) && function->GetName() == STR("ReceiveTick")) {
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
                Client::SendCheck(context.GetParams<int64_t>(), Engine::GetWorldName());
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
                    Logger::Log(L"Could not find function \"ReturnCheck\" in BP_APCollectible.", Logger::LogType::Error);
                    return;
                }
                Unreal::UObjectGlobals::RegisterHook(return_check_function, EmptyFunction, returncheck, nullptr);
                returncheck_hooked = true;
            }

            if (actor->GetName().starts_with(STR("BP_APRandomizerInstance"))) {
                if (!toggleslidejump_hooked) {
                    UFunction* toggle_function = actor->GetFunctionByName(L"AP_ToggleSlideJump");
                    if (!toggle_function) {
                        Logger::Log(L"Could not find function \"AP_ToggleSlideJump\" in BP_APRandomizerInstance.", Logger::LogType::Error);
                        return;
                    }
                    Unreal::UObjectGlobals::RegisterHook(toggle_function, EmptyFunction, toggleslidejump, nullptr);
                    toggleslidejump_hooked = true;
                }
                Logger::Log(L"Loaded scene " + Engine::GetWorldName());
                if (Engine::GetWorldName() == STR("EndScreen")) {
                    Client::CompleteGame();
                }
                Engine::SpawnCollectibles();
                Engine::SyncItems();
            }

            if (actor->GetName().starts_with(L"BP_PlayerGoatMain")) {
                if (!deathlink_hooked) {
                    UFunction* death_function = actor->GetFunctionByName(L"BPI_CombatDeath");
                    if (!death_function) {
                        Logger::Log(L"Could not find function \"BPI_CombatDeath\" in BP_PlayerGoatMain.", Logger::LogType::Error);
                        return;
                    }
                    Unreal::UObjectGlobals::RegisterHook(death_function, EmptyFunction, deathlink, nullptr);
                }
            }
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