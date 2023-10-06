#pragma once
#include "DynamicOutput/DynamicOutput.hpp"
#include "APClient.hpp"
#include <iostream>

namespace Pseudoregalia_AP {
	class APConsoleManager {
	public:
		static void ProcessCommand(const Unreal::TCHAR*);
	private:
		static const char DELIM;
		static void ParseConnect(std::string);
		static void ParseMessageOption(std::string);
		static std::string GetNextToken(std::string&);
		static std::string ConvertTcharToString(const Unreal::TCHAR*);
	};
}