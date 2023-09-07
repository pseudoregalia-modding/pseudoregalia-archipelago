#pragma once
#include "Archipelago.h"
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include "APCollectible.hpp"

namespace Pseudoregalia_AP {
	class APClient {
	public:
		APClient();
		void Connect(const char*, const char*, const char*);
		void SendCheck(int);
		void OnMapLoad(AActor*, UFunction*);
	private:
		void Initialize();
	};
}