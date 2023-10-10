#pragma once
#include <map>
#include <vector>
#include <string>
#include "APCollectible.hpp"

using namespace RC::Unreal;

namespace GameData {
	enum class ItemType {
		Ability,
		HealthPiece,
		SmallKey,
		MajorKey,
		Unknown
	};

	void Initialize();
	int GetHealthPieces();
	int GetSmallKeys();
	bool* GetMajorKeys();
	std::map<std::wstring, int> GetUpgradeTable();
	std::vector<APCollectible> GetCollectiblesOfZone(std::wstring);
	void CheckLocation(int64_t);
	void ReceiveItem(int64_t);


	ItemType GetItemType(int64_t);
	int health_pieces;
	int small_keys;
	bool major_keys[5];
	std::map<std::wstring, int> upgrade_table;
	std::map<std::wstring, std::vector<APCollectible>> collectible_table;
	const std::map<int64_t, std::wstring> lookup_id_to_upgrade = {
		{2365810001, L"attack"},
		{2365810002, L"powerBoost"},
		{2365810003, L"airKick"},
		{2365810004, L"slide"},
		{2365810005, L"SlideJump"},
		{2365810006, L"plunge"},
		{2365810007, L"chargeAttack"},
		{2365810008, L"wallRide"},
		{2365810009, L"Light"},
		{2365810010, L"projectile"},
		{2365810011, L"extraKick"},
		{2365810012, L"airRecovery"},
		{2365810013, L"mobileHeal"},
		{2365810014, L"magicHaste"},
		{2365810015, L"healBoost"},
		{2365810016, L"damageBoost"},
		{2365810017, L"magicPiece"},
		{2365810018, L"outfitPro"},
	};
}