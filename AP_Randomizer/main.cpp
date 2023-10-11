#define NOMINMAX
#pragma once

#include <Windows.h>
#include "Mod/CppUserModBase.hpp"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/Hooks.hpp"
#include "Unreal/UFunction.hpp"
#include "Client.hpp"
#include "UnrealConsole.hpp"
#include "Engine.hpp"
#include "Logger.hpp"

class AP_Randomizer : public RC::CppUserModBase {
public:
    // Probably remove direct keybinds like this outside of debugging
    struct BoundKey {
        int key;
        std::function<void()> callback;
        bool isPressed = false;
    };

public:

    bool returncheck_hooked = false;

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

    auto on_unreal_init() -> void override
    {
        using namespace RC::Unreal;

        // I want to make this an AActorTickCallback hook so I can use delta_seconds and only check actor name,
        // but for some reason that doesn't seem to respond.
        // Might check with UE4SS devs on that.
        Hook::RegisterProcessEventPreCallback([&](UObject* object, UFunction* function, void* params) {
            if (object->GetName().starts_with(STR("BP_APRandomizerInstance")) && function->GetName() == STR("ReceiveTick")) {
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

        setup_keybinds();

        Hook::RegisterBeginPlayPostCallback([&](AActor* actor) {
            // TODO: Consider moving some of this function out of main
            auto returncheck = [](UnrealScriptFunctionCallableContext& context, void* customdata) {
                Client::SendCheck(context.GetParams<int64_t>(), Engine::GetWorld()->GetName());
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
                Logger::Log(L"Loaded scene " + Engine::GetWorld()->GetName());
                if (Engine::GetWorld()->GetName() == STR("EndScreen")) {
                    Client::CompleteGame();
                }
                Engine::SpawnCollectibles();
                Engine::SyncItems();
            }
            });
    }

    static void EmptyFunction(RC::Unreal::UnrealScriptFunctionCallableContext& context, void* customdata) {

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