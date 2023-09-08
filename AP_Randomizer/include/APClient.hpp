#pragma once
#include "Archipelago.h"
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include "APCollectible.hpp"

namespace Pseudoregalia_AP {
	class APClient {
	public:
		static std::map<int64_t, std::string> lookup_id_to_item;
		std::map <std::wstring, std::vector<APCollectible>> zone_table;
		APClient();
		void FillZoneTable();
		void Connect(const char*, const char*, const char*);
		void SendCheck(int64_t, std::wstring);
		void OnMapLoad(AActor*, UFunction*, std::wstring);
	private:
		void Initialize();
	};
}