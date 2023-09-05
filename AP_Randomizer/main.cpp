#define NOMINMAX

#include <Windows.h>
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/World.hpp>
#include <Unreal/TMap.hpp>
#include "Archipelago.h"

#include "APClient.hpp"
#include "GoatManager.hpp"

using namespace RC;
using namespace RC::Unreal;
using namespace Pseudoregalia_AP;

class AP_Randomizer : public RC::CppUserModBase {

    APClient* client = new APClient();

public:
    // Probably remove direct keybinds like this outside of debugging
    struct BoundKey {
        int key;
        std::function<void()> callback;
        bool isPressed = false;
    };

    bool isHooked = false;

public:
    AP_Randomizer() : CppUserModBase() {
        ModName = STR("AP_Randomizer");
        ModVersion = STR("0.1.0");
        ModDescription = STR("archipelago randomizer for pseudoregalia");
        ModAuthors = STR("LittleMeowMeow0134");
        //ModIntendedSDKVersion = STR("2.6");
    }

    ~AP_Randomizer()
    {
    }

    auto on_unreal_init() -> void override
    {
        // TODO: add proper input for client instead of just keypresses
        // TODO: here, initalize whatever input system for client alongside the client(?)
        
        // Initialize GoatManager; it needs to hook into collectible collision function and talk to the client
        GoatManager* manager = new GoatManager(client);


        setup_keybinds();

        Hook::RegisterBeginPlayPostCallback([&](AActor* Actor) {
            this->on_begin_play(Actor);
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
            client->Connect("localhost:38281", "goat", "");
            });

        bind_key(VK_NUMPAD2, [&]() {
            });

        bind_key(VK_NUMPAD7, [&]() {
            AP_SendItem(4206942067);
            });

        bind_key(VK_NUMPAD8, [&]() {
            AP_StoryComplete();
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

    auto test_spawn_actor() -> void
    {
        //static UClass* ClassToSpawn = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/Engine.PointLight"));
        //static AActor* playerActor = UObjectGlobals::StaticFindObject<AActor*>(nullptr, nullptr, STR("/Game/ThirdPerson/Player/BP_PlayerGoatMain.BP_PlayerGoatMain_C"));

        // TODO try to change all instances of StaticFindObject with FindObject
        const wchar_t* apCollectibleClassName = STR("/Game/Mods/AP_Randomizer/AP_Collectible.AP_Collectible_C");
        static UClass* apCollectibleClass = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, apCollectibleClassName);

        if (!apCollectibleClass) {
            Output::send<LogLevel::Error>(STR("Could not find class named {}."), apCollectibleClassName);
            return;
        }

        // Get the current level we're in
        auto World = static_cast<UWorld*>(UObjectGlobals::FindFirstOf(STR("World")));

        // hardcoded locations for upgrades for now
        // there seem to be three upgrades in dungeon, not sure if one is the minor pickup
        auto apSpawnLocation1 = FVector(-3500, 4950, -50);
        auto apSpawnLocation2 = FVector(16650, 2600, 2350);
        auto apSpawnLocation3 = FVector(18250, -9750, 4200);

        // Rotation will always be (0, 0, 0) in Unreal Engine 4 games, this is an issue with UE4SS and will be fixed in the future
        // Workaround is to replicate SpawnActor and have it take FVector::AsPre500 instead of FVector
        // Unreal Engine 5 games should behave normally
        auto apRotation = FRotator(0.f, 0.f, 0.f);

        World->SpawnActor(
            apCollectibleClass,
            &apSpawnLocation1,
            &apRotation
        );

        World->SpawnActor(
            apCollectibleClass,
            &apSpawnLocation2,
            &apRotation
        );

        World->SpawnActor(
            apCollectibleClass,
            &apSpawnLocation3,
            &apRotation
        );

        Output::send<LogLevel::Verbose>(STR("Spawned upgrade actors (i hope!)"));

        //Output::send<LogLevel::Verbose>(STR("[{}] Actor Spawned {}\n"), ModName, SpawnedActor->GetName());
    }

private:
    std::vector<BoundKey> m_boundKeys;
    std::unordered_set<int> m_pressedKeys;

    // Called whenever an actor is spawned
    auto on_begin_play(AActor* Actor) -> void
    {
        // Look for APRandomizerInstance
        if (Actor->GetName().starts_with(STR("APRandomizerInstance"))) {
            Output::send<LogLevel::Verbose>(STR("[{}] Found APRandomizerInstance.\n"), ModName);

            // Hook to blueprint connect function, but only once
            Unreal::UFunction* pAPTryConnectFunction = nullptr;
            pAPTryConnectFunction = Actor->GetFunctionByName(STR("AP_TryConnect"));
            if (pAPTryConnectFunction) {
                if (!isHooked) {
                    Unreal::UObjectGlobals::RegisterHook(pAPTryConnectFunction, APConnectPrehook, APConnectPosthook, nullptr);
                    Output::send<LogLevel::Verbose>(STR("Hooked into AP_TryConnectFunction."), ModName);
                    isHooked = true;
                }
            }
            else {
                //Print an error if APTryConnectFunction doesn't exist.
                Output::send<LogLevel::Warning>(STR("APRandomizerInstance was found, but had no function AP_TryConnect.\n"), ModName);
            }
        }
    }

    // Called on the blueprint's connect function.
    static void APConnectPrehook(Unreal::UnrealScriptFunctionCallableContext& Context, void* CustomData) {
        Output::send<LogLevel::Verbose>(STR("AP_TryConnect was called."));
        AP_Init("localhost:38281", "Pseudoregalia", "goat", "");

        AP_SetItemClearCallback(&ClearItems);
        AP_SetItemRecvCallback(&RecvItems);
        AP_SetLocationCheckedCallback(&CheckLocation);

        AP_Start();
        Output::send<LogLevel::Verbose>(STR("I probably just crashed."));
    }

    static void ClearItems() {

    }

    static void RecvItems(int64_t index, bool notify) {
        
    }

    static void CheckLocation(int64_t loc_id) {

    }

    static void APConnectPosthook(Unreal::UnrealScriptFunctionCallableContext& Context, void* CustomData) {
        // Output::send<LogLevel::Verbose>(STR("APTryConnect was post called!! :3"));
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