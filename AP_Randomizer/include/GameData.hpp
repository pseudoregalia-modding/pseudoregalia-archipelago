#pragma once
#include <map>
#include <vector>
#include "Collectible.hpp"

namespace GameData {
	const int MAP_PATCH = 1;
	const int FULL_GOLD = 2;

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

	namespace EPseudoType {
		enum Type {
			MajorAbility = 0,
			MinorAbility = 1,
			HealthPiece = 2,
			MajorKey = 3,
			SmallKey = 4,
			OffWorld = 5,
			EPseudoType_MAX = 6,
		};
	}

	namespace EClassification {
		enum Type {
			Progression = 0,
			Useful = 1,
			Filler = 2,
			Unknown = 3,
			EClassification_MAX = 4,
		};
	}

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
	// encodes item type data
	typedef std::pair<EPseudoType::Type, EClassification::Type> ItemType;

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
	void SetPseudoItemType(int64_t, int64_t, EClassification::Type);
	void SetOffWorldItemType(int64_t, EClassification::Type);
	ItemType GetItemType(int64_t);
	void ResetItems();
	void ReceiveItem(int64_t, bool);
	Map MapNameToEnum(std::wstring);
	bool ToggleSlideJump();
	bool SlideJumpDisabled();
	bool CanHaveTimeTrial(Map);
	std::optional<TimeTrial> GetTimeTrial(Map, std::wstring);
	void Interact(std::wstring);
	void ReadNote(std::wstring);
	void FinishNote();
	std::optional<std::wstring> GetNoteText(std::wstring);
	bool IsInteractable(int64_t);
	void AddMajorKeyHint(int, MultiworldLocation);
	std::optional<MajorKeyInfo> GetMajorKeyInfo(std::wstring);
	const PlayerStart& GetSpawnInfo();
}