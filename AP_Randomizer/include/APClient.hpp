#pragma once
#include "Archipelago.h"
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include "APCollectible.hpp"

namespace Pseudoregalia_AP {
	class APClient {
	public:
		static void Initialize();
		static void Connect(const char*, const char*, const char*);
		static void SendCheck(int64_t, std::wstring);
		static void OnMapLoad(AActor*, UFunction*, std::wstring);
	private:
		static void FillZoneTable();
		static void ResetUpgradeTable();
		static void ClearItems();
		static void ReceiveItem(int64_t, bool);
		static void CheckLocation(int64_t);
		static std::map<int64_t, std::string> lookup_id_to_item;
		static std::map <std::wstring, std::vector<APCollectible>> zone_table;
		static std::map<std::string, int> upgrade_table;
	};
}