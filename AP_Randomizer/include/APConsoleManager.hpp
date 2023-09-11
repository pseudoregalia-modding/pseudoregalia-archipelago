#pragma once
#include "DynamicOutput/DynamicOutput.hpp"
#include "APClient.hpp"

namespace Pseudoregalia_AP {
	class APConsoleManager {
	public:
		static void ProcessCommand(const Unreal::TCHAR*);
	private:
		static std::string ConvertTcharToString(const Unreal::TCHAR*);
	};
}