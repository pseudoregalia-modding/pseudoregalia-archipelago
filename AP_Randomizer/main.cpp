#define NOMINMAX
#pragma once

#include <Windows.h>
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/World.hpp>
#include "APClient.hpp"
#include "APGameManager.hpp"

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

    bool hooked_into_apconnect = false;
    bool hooked_into_collectible = false;
    bool hooked_into_initgamestate = false;
    bool hooked_into_randomizerbeginplay = false;

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
        if (!hooked_into_initgamestate) {
            Hook::RegisterInitGameStatePostCallback([&](AActor* Actor) {
                this->OnSceneLoad();
                });
        }
        
        APClient::Initialize();

        Hook::RegisterProcessEventPreCallback([&](UObject* object, UFunction* function, void* params) {
            APClient::PreProcessEvent(object, function, params);
            });

        setup_keybinds();

        Hook::RegisterBeginPlayPostCallback([&](AActor* Actor) {
            APGameManager::OnBeginPlay(Actor);
            });
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

    // Called whenever an actor is spawned
    auto on_begin_play(AActor* Actor) -> void
    {
        if (!hooked_into_collectible) {
            if (Actor->GetName().starts_with(STR("BP_APCollectible"))) {
                Output::send<LogLevel::Verbose>(STR("[{}] Found BP_APCollectible.\n"), ModName);

                auto ReturnCheckLambda = [&](Unreal::UnrealScriptFunctionCallableContext& Context, void* CustomData) {
                    ReturnCheckPrehook(Context, CustomData);
                    };

                auto ReturnCheckFunction = UObjectGlobals::StaticFindObject<UFunction*>(nullptr, nullptr, STR("/Game/Mods/AP_Randomizer/BP_APCollectible.BP_APCollectible_C:ReturnCheck"));
                if (ReturnCheckFunction) {
                    Unreal::UObjectGlobals::RegisterHook(ReturnCheckFunction, ReturnCheckLambda, ReturnCheckPosthook, nullptr);
                    Output::send<LogLevel::Verbose>(STR("Hooked into ReturnCheck."), ModName);
                    hooked_into_collectible = true;
                }
                else {
                    Output::send<LogLevel::Error>(STR("BP_APCollectible was found, but had no function ReturnCheck.\n"), ModName);
                }
            }
        }
    }

    static void APConnectPrehook(Unreal::UnrealScriptFunctionCallableContext& Context, void* CustomData) {
        APClient::Connect("localhost:38281", "goat", "");
    }

    void ReturnCheckPrehook(Unreal::UnrealScriptFunctionCallableContext& Context, void* CustomData) {
        struct ReturnCheckParams {
            int64_t id;
        };

        auto& params = Context.GetParams<ReturnCheckParams>();
        Output::send<LogLevel::Verbose>(STR("Obtained check with ID {}\n"), params.id);

        UWorld* World = APGameManager::GetWorld();
        APClient::SendCheck(params.id, World->GetName());
    }

    static void ReturnCheckPosthook(Unreal::UnrealScriptFunctionCallableContext& Context, void* CustomData) {
        // Placeholder so the game doesn't crash when trying to run a ReturnCheck posthook.
    }

    void OnSceneLoad() {
        Output::send<LogLevel::Verbose>(STR("InitGameState called."));
    }
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