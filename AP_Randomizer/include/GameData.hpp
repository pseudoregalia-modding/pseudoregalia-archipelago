#pragma once
#include <map>
#include <vector>
#include "Collectible.hpp"

namespace GameData {
	const int MAP_PATCH = 1;
	const int FULL_GOLD = 2;

	enum class ItemType {
		MajorAbility,
		MinorAbility,
		HealthPiece,
		SmallKey,
		MajorKey,
		Filler,
		Unknown
	};

	enum class Map {
		TitleScreen,
		Dungeon,
		Castle,
		Keep,
		Library,
		Theatre,
		Bailey,
		Underbelly,
		Tower,
		Chambers,
		EndScreen,
	};

	// this is an enum instead of an enum class because we cast to an int when
	// calling the spawn collectible blueprint
	enum Classification {
		Generic,
		GenericProgression,
		GenericUsefulOrTrap,
		GenericFiller,
		MajorAbility,
		MinorAbility,
		HealthPiece,
		MajorKey,
		SmallKey,
	};

	struct MultiworldLocation {
		int player_id;
		int64_t location_id;
	};

	struct MajorKeyInfo {
		int64_t item_id;
		bool found;
		std::vector<MultiworldLocation> locations;
	};

	struct PlayerStart {
		std::wstring zone;
		std::wstring player_start;
		std::wstring spawn_name;
	};

	// encodes the location id and spawn position of a time trial collectible
	typedef std::pair<int64_t, FVector> TimeTrial;
	// encodes the location id and actor class name of an interactable location
	typedef std::pair<int64_t, std::wstring> Interactable;

	void Initialize();
	void Close();
	int GetHealthPieces();
	int GetSmallKeys();
	bool* GetMajorKeys();
	void SetOption(std::string, int);
	std::unordered_map<std::string, int> GetOptions();
	std::unordered_map<std::wstring, int> GetUpgradeTable();
	std::unordered_map<int64_t, Collectible> GetCollectiblesOfZone(Map);
	std::unordered_map<std::wstring, Interactable> GetInteractablesOfZone(Map);
	std::list<int64_t> GetMissingSpawnableLocations();
	void SetPseudoItemClassification(int64_t, int64_t);
	void SetOffWorldItemClassification(int64_t, Classification);
	Classification GetClassification(int64_t);
	void ResetItems();
	ItemType ReceiveItem(int64_t);
	Map MapNameToEnum(std::wstring);
	bool ToggleSlideJump();
	bool SlideJumpDisabled();
	bool CanHaveTimeTrial(Map);
	std::optional<TimeTrial> GetTimeTrial(Map, std::wstring);
	void Interact(std::wstring);
	void ReadNote(std::wstring);
	void FinishNote();
	std::optional<std::wstring> GetNoteText(std::wstring);
	void ReceiveItemOnce(int64_t);
	bool IsInteractable(int64_t);
	void AddMajorKeyHint(int, MultiworldLocation);
	std::optional<MajorKeyInfo> GetMajorKeyInfo(std::wstring);
	const PlayerStart& GetSpawnInfo();
}