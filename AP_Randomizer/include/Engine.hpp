#pragma once
#include <variant>
#include "Unreal/UObject.hpp"
#include "GameData.hpp"

namespace Engine {
	using RC::Unreal::UObject;

	struct ItemPopup {
		std::wstring preamble;
		std::wstring item;
		std::wstring info;
	};

	typedef std::array<int32_t, 3> Version;
	std::wstring VersionToWString(const Version&);
	bool IsAPWorldVersionCompatible(const Version&);
	void CheckVersionCompatibility(RC::Unreal::UnrealScriptFunctionCallableContext&);

	void ExecuteBlueprintFunction(std::variant<std::wstring, UObject*>, std::wstring, std::shared_ptr<void>);
	void OnTick(UObject*);
	void OnSceneLoad(UObject*);
	void SyncItems();
	void SpawnCollectibles(GameData::Map);
	void DespawnCollectible(const int64_t);
	void DespawnInteractable(const int64_t);
	GameData::Map GetCurrentMap(UObject*);
	GameData::Map GetCurrentMap();
	void ToggleSlideJump();
	void VaporizeGoat();
	void SpawnTimeTrialCollectibleIfBeaten(UObject*);
	void PrintToConsole(std::wstring, std::wstring);
	void PrintToConsole(std::wstring);
	void ShowPopup(std::variant<std::wstring, ItemPopup>);
	void TogglePopupsMute();
	void TogglePopupsHide();
	void HealPlayer();
	void GivePlayerPower();
	void WarpToSpawn();
	void SetTombstoneText(UObject*);
	void CreateMajorKeyHints(UObject*);
	void Init();
	void StartConnectHandshake(UObject*);
	bool IsInConnectHandshake();
	void UpdateConnectHandshakeStatus(std::wstring, bool);
	void FinishConnect(std::wstring, std::wstring, std::wstring, std::wstring, Version);
	void FinishConnect();
	void EndConnectHandshake();
	void RecallBreaker();
}