#pragma once

#include "Unreal/AActor.hpp"

namespace ModHooks {
	void RegisterActorHooks(RC::Unreal::AActor*);
	void RunBeginPlayPostCallback(RC::Unreal::AActor*);
	void RegisterObjectHooks(RC::Unreal::UObject*);
	void RunStaticConstructObjectPostCallback(RC::Unreal::UObject*);
}
