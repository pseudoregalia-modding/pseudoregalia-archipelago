#define NOMINMAX
#pragma once

#include <Windows.h>
#include "Mod/CppUserModBase.hpp"
#include "Client.hpp"
#include "GameManager.hpp"
#include "UnrealConsole.hpp"
#include "Engine.hpp"
#include "Unreal/UObjectGlobals.hpp"

using namespace RC;
using namespace RC::Unreal;

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
        Hook::RegisterProcessEventPreCallback([&](UObject* object, UFunction* function, void* params) {
            if (object->GetName().starts_with(STR("BP_APRandomizerInstance")) && function->GetName() == STR("ReceiveTick")) {
                Engine::OnTick(object);
            }
            // APGameManager::PreProcessEvent(object, function, params);
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

        Hook::RegisterBeginPlayPostCallback([&](AActor* Actor) {

            // TODO: There should probably be less of this function in main
            auto returncheck = [](UnrealScriptFunctionCallableContext& context, void* customdata) {
                struct return_check_params {
                    int64_t id;
                };
                auto& params = context.GetParams<return_check_params>();
                Client::SendCheck(params.id, Engine::GetWorld()->GetName());
                };

            if (!returncheck_hooked
                && Actor->GetName().starts_with(STR("BP_APCollectible"))) {

                UFunction* return_check_function = Actor->GetFunctionByName(STR("ReturnCheck"));
                if (!return_check_function) {
                    Output::send<LogLevel::Error>(STR("Could not find function ReturnCheck in BP_APCollectible."));
                    return;
                }
                Unreal::UObjectGlobals::RegisterHook(return_check_function, GameManager::EmptyFunction, returncheck, nullptr);
                returncheck_hooked = true;
            }

            GameManager::OnBeginPlay(Actor);
            });
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
            // APClient::Connect("localhost:38281", "goat", "");
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
        GameManager::OnUpdate();
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