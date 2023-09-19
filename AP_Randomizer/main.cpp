#define NOMINMAX
#pragma once

#include <Windows.h>
#include "Mod/CppUserModBase.hpp"
#include "APClient.hpp" // Currently is only included to connect on keypress
#include "APGameManager.hpp"
#include "APConsoleManager.hpp"

using namespace RC;
using namespace RC::Unreal;
using namespace Pseudoregalia_AP;

class AP_Randomizer : public RC::CppUserModBase {
public:
    // Probably remove direct keybinds like this outside of debugging
    struct BoundKey {
        int key;
        std::function<void()> callback;
        bool isPressed = false;
    };

public:
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
        APClient::Initialize();

        Hook::RegisterProcessEventPreCallback([&](UObject* object, UFunction* function, void* params) {
            APGameManager::PreProcessEvent(object, function, params);
            });

        Hook::RegisterProcessConsoleExecCallback([&](UObject* object, const Unreal::TCHAR* command, FOutputDevice& Ar, UObject* executor) -> bool {
            if (command[0] == '/' || command[0] == '!') {
                command++; // Exclude the first character from the array
                APConsoleManager::ProcessCommand(command);
                return true;
            }
            return PropogateCommand(command);
            });

        setup_keybinds();

        Hook::RegisterBeginPlayPostCallback([&](AActor* Actor) {
            APGameManager::OnBeginPlay(Actor);
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
            APClient::Connect("localhost:38281", "goat", "");
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
        APClient::PollServer();
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