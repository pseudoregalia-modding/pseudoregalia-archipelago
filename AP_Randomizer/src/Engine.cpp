#pragma once
#include <mutex>
#include <queue>
#include <unordered_set>
#include "Unreal/TArray.hpp"
#include "Unreal/World.hpp"
#include "Engine.hpp"
#include "Logger.hpp"
#include "Client.hpp"
#include "Settings.hpp"
#include "StringOps.hpp"
#include "Timer.hpp"

namespace Engine {
	using namespace RC::Unreal; // Give Engine easy access to Unreal objects
	using std::variant;
	using std::wstring;
	using std::to_wstring;
	using std::shared_ptr;
	using std::mutex;
	using std::lock_guard;
	using std::get;
	using std::unordered_set;
	using std::vector;
	using std::optional;
	using std::deque;
	using std::pair;

	// Private members
	namespace {
		void QueueItemSync();
		void SyncMajorKeys();
		void SyncHealthPieces();
		void SyncSmallKeys();
		void SyncAbilities();
		void SpawnCollectible(int64_t, FVector);
		void SpawnInteractableAura(wstring, GameData::Interactable);
		void AddMessages(UObject*);
		void ShowQueuedPopup(UObject*);
		void CreateOverlay(UObject*);
		void VerifyGameVersion(GameData::Map);
		
		// version compatibility stuff
		const size_t MAJOR = 0;
		const size_t MINOR = 1;
		const size_t PATCH = 2;
		const int32_t PRE_RELEASE = 0;
		const Version client_version = { 0, 10, 1 };
		const wstring client_version_text = VersionToWString(client_version);

		// keeps track of collectibles spawned since the last time SpawnCollectibles was called. this is necessary because
		// time trials may try to spawn their collectibles multiple times if the player beats the time trial more than once
		// without getting the collectible
		unordered_set<int64_t> spawned_collectibles;

		// matches the max amount of messages the console widget will display
		const size_t max_messages = 100;
		const auto message_debounce_delay = std::chrono::milliseconds(300);
		size_t queued_messages = 0;
		deque<pair<wstring, wstring>> messages;
		bool message_debounce_locked;
		mutex messages_mutex;

		const auto popup_debounce_delay = std::chrono::milliseconds(500);
		bool popups_muted;
		bool popups_hidden;
		optional<variant<wstring, ItemPopup>> queued_popup;
		bool popup_debounce_locked;
		mutex popups_mutex;

		bool item_sync_queued;
		mutex item_sync_mutex;

		struct BlueprintFunctionInfo {
			variant<wstring, UObject*> parent;
			wstring function_name;
			shared_ptr<void> params;
		};
		std::queue<BlueprintFunctionInfo> blueprint_function_queue;
		mutex blueprint_function_mutex;

		optional<UObject*> file_object;
		mutex file_object_mutex;

		bool verified_version = false;
	} // End private members


	// Returns the current map as a Map enum, getting the world name from the player controller.
	GameData::Map GetCurrentMap() {
		static FName controller_name = FName(STR("PlayerController"));
		UObject* player_controller = UObjectGlobals::FindFirstOf(controller_name);
		return GetCurrentMap(player_controller);
	}

	// Returns the current map as a Map enum, getting the world name from the object.
	GameData::Map GetCurrentMap(UObject* object) {
		wstring world_name = object->GetWorld()->GetName();
		return GameData::MapNameToEnum(world_name);
	}

	// Queues up a blueprint function to be executed.
	void Engine::ExecuteBlueprintFunction(variant<wstring, UObject*> new_parent, wstring new_name, shared_ptr<void> params) {
		lock_guard<mutex> guard(blueprint_function_mutex);
		blueprint_function_queue.push(BlueprintFunctionInfo(new_parent, new_name, params));
	}

	// Runs once every engine tick.
	void Engine::OnTick(UObject* ap_object) {
		QueueItemSync();
		AddMessages(ap_object);
		ShowQueuedPopup(ap_object);

		// Engine tick runs in a separate thread from the client so it needs to be locked.
		lock_guard<mutex> guard(blueprint_function_mutex);
		for (; !blueprint_function_queue.empty(); blueprint_function_queue.pop()) {
			BlueprintFunctionInfo info = blueprint_function_queue.front();
			UObject* object;
			if (std::holds_alternative<wstring>(info.parent)) {
				wstring parent_name = get<wstring>(info.parent);
				if (parent_name == L"BP_APRandomizerInstance_C") {
					object = ap_object;
				}
				else {
					object = UObjectGlobals::FindFirstOf(parent_name);
					if (!object) {
						Log(L"Could not find blueprint with name " + parent_name, LogType::Error);
						continue;
					}
				}
			}
			else {
				object = get<UObject*>(info.parent);
				if (object->IsUnreachable()) {
					Log(L"Could not call " + info.function_name + L" because the blueprint was unreachable.", LogType::Error);
					continue;
				}
			}

			UFunction* function = object->GetFunctionByName(info.function_name.c_str());
			if (!function) {
				Log(L"Could not find function " + info.function_name, LogType::Error);
				continue;
			}
			Log(L"Executing " + info.function_name);
			// Need to cast to raw pointer to feed to ProcessEvent, but the memory will still be freed automatically
			void* ptr(info.params.get());
			object->ProcessEvent(function, ptr);
		}
	}

	// Performs actions that should be done at the start of a new scene
	void OnSceneLoad(UObject* ap_object) {
		GameData::Map map = GetCurrentMap(ap_object);
		if (map == GameData::Map::EndScreen) {
			ExecuteBlueprintFunction(ap_object, L"AP_MarkGameCompleted", nullptr);
			Client::CompleteGame();
			return;
		}
		if (map == GameData::Map::TitleScreen) {
			Client::Disconnect();
			CreateOverlay(ap_object);
            // Normally we create the console in response to the HUD being constructed in a gameplay level. For some
            // reason, if we don't create the console on the title screen, it causes issues with the map item. Maybe one
            // day I will figure out why tf that happens.
			ExecuteBlueprintFunction(ap_object, L"AP_CreateConsoleHacky", nullptr);
			verified_version = false;
			lock_guard<mutex> guard(popups_mutex);
			queued_popup = {};
			return;
		}
		VerifyGameVersion(map);
		Engine::SpawnCollectibles(map);
		Engine::SyncItems();
		Client::SetZoneData(map);
	}

	// Calls blueprint's AP_SpawnCollectible function for each unchecked collectible in a map.
	void Engine::SpawnCollectibles(GameData::Map map) {
		spawned_collectibles.clear();
		// This function must loop through instead of calling once with an array;
		// as of 10/11/23 the params struct method I use can't easily represent FVectors or FTransforms in C++.
		// This might be worked around by storing positions as three separate numbers instead and constructing the vectors in BP,
		// but I don't think it's worth changing right now since this is just called once each map load.
		std::unordered_map<int64_t, GameData::Collectible> collectible_map = GameData::GetCollectiblesOfZone(map);
		for (const auto& [id, collectible] : collectible_map) {
			SpawnCollectible(id, collectible.GetPosition(GameData::GetOptions()));
		}

		if (Settings::GetInteractableAuraDisplay() == Settings::InteractableAuraDisplay::None) {
			return;
		}
		std::unordered_map<wstring, GameData::Interactable> interactable_map = GameData::GetInteractablesOfZone(map);
		for (const auto& [name, interactable] : interactable_map) {
			SpawnInteractableAura(name, interactable);
		}
	}

	// Queues all item sync functions.
	void Engine::SyncItems() {
		lock_guard<mutex> guard(item_sync_mutex);
		item_sync_queued = true;
	}

	void Engine::ToggleSlideJump() {
		if (GameData::ToggleSlideJump()) {
			SyncAbilities();
		}
	}

	// Kills Sybil.
	void Engine::VaporizeGoat() {
		shared_ptr<void> dissolve_delay(new double(0));
		ExecuteBlueprintFunction(L"BP_PlayerGoatMain_C", L"BPI_CombatDeath", dissolve_delay);
	}

	void Engine::DespawnCollectible(const int64_t id) {
		std::vector<UObject*> collectibles{};
		UObjectGlobals::FindAllOf(STR("BP_APCollectible_C"), collectibles);
		for (auto const collectible : collectibles) {
			void* property_ptr = collectible->GetValuePtrByPropertyName(STR("id"));
			int64_t* new_id = static_cast<int64_t*>(property_ptr);
			if (*new_id == id) {
				Log(L"Manually despawning collectible with id " + to_wstring(id));
				ExecuteBlueprintFunction(collectible, L"Despawn", nullptr);
				break;
			}
			// It's fine if we don't find the collectible, it could just be in another map or already despawned
		}
	}

	void DespawnInteractable(const int64_t id) {
		std::vector<UObject*> collectibles{};
		UObjectGlobals::FindAllOf(STR("BP_APInteractableAura_C"), collectibles);
		for (auto const collectible : collectibles) {
			void* property_ptr = collectible->GetValuePtrByPropertyName(STR("interactableId"));
			int64_t* new_id = static_cast<int64_t*>(property_ptr);
			if (*new_id == id) {
				Log(L"Manually despawning interactable aura with id " + to_wstring(id));
				ExecuteBlueprintFunction(collectible, L"Despawn", nullptr);
				break;
			}
		}
	}

	void SpawnTimeTrialCollectibleIfBeaten(UObject* obj) {
		wstring name = obj->GetName();
		optional<GameData::TimeTrial> time_trial = GameData::GetTimeTrial(GetCurrentMap(), name);
		if (!time_trial) {
			Log(L"Collectible not found for time trial " + name);
			return;
		}
		auto& [id, position] = *time_trial;
		int32_t medal_tier = *static_cast<int32_t*>(obj->GetValuePtrByPropertyName(L"medalTier"));
		if (medal_tier < 1) {
			Log(L"Time trial for collectible " + to_wstring(id) + L" has not been beaten");
			return;
		}
		SpawnCollectible(id, position);
	}

	void PrintToConsole(wstring markdown_text, wstring plain_text) {
		Log(plain_text, LogType::Console);
		lock_guard<mutex> guard(messages_mutex);
		if (messages.size() == max_messages) {
			messages.pop_front();
		}
		messages.push_back({ markdown_text, plain_text });
		if (queued_messages < max_messages) {
			queued_messages++;
		}
	}

	void PrintToConsole(wstring text) {
		PrintToConsole(text, text);
	}

	void ShowPopup(variant<wstring, ItemPopup> popup) {
		lock_guard<mutex> guard(popups_mutex);
		if (popups_hidden) return;
		queued_popup = popup;
	}

	void TogglePopupsMute() {
		lock_guard<mutex> guard(popups_mutex);
		popups_muted = !popups_muted;
		if (popups_muted) {
			Log(L"Popup sounds are now muted.", LogType::System);
		}
		else {
			Log(L"Popup sounds are no longer muted.", LogType::System);
		}
	}

	void TogglePopupsHide() {
		lock_guard<mutex> guard(popups_mutex);
		popups_hidden = !popups_hidden;
		if (popups_hidden) {
			queued_popup = {};
			Log(L"Popups are now hidden.", LogType::System);
		}
		else {
			Log(L"Popups are no longer hidden.", LogType::System);
		}
	}

	void HealPlayer() {
		GameData::Map map = GetCurrentMap();
		if (map == GameData::Map::TitleScreen || map == GameData::Map::EndScreen) {
			// don't try to heal unless in a gameplay level
			return;
		}

		shared_ptr<void> Amount(new double(10));
		ExecuteBlueprintFunction(L"BP_PlayerGoatMain_C", L"healPlayer", Amount);
	}

	void GivePlayerPower() {
		GameData::Map map = GetCurrentMap();
		if (map == GameData::Map::TitleScreen || map == GameData::Map::EndScreen) {
			// don't try to give power unless in a gameplay level
			return;
		}

		struct ChangePowerAmountInfo {
			double A;
			bool forceUpdatePowerLevel;
		};
		shared_ptr<void> power_params(new ChangePowerAmountInfo{ 10, false });
		ExecuteBlueprintFunction(L"BP_PlayerGoatMain_C", L"changePowerAmount", power_params);
		ExecuteBlueprintFunction(L"BP_PlayerGoatMain_C", L"updatePlayerCurrentStatValues", nullptr);
	}

	void WarpToSpawn() {
		const auto& spawn_info = GameData::GetSpawnInfo();
		struct WarpInfo {
			FString zone;
			FString playerStart;
		};
		shared_ptr<void> warp_params(new WarpInfo{
			FString(spawn_info.zone.c_str()),
			FString(spawn_info.player_start.c_str()),
		});
		ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_Warp", warp_params);
	}

	void SetTombstoneText(UObject* object) {
		if (GetCurrentMap(object) != GameData::Map::Tower) return;

		wstring tombstone_name = object->GetName();
		optional<GameData::MajorKeyInfo> info = GameData::GetMajorKeyInfo(tombstone_name);
		if (!info) {
			return;
		}

		if (info->locations.size() == 0) {
			Log(L"No hints for tower tombstone " + tombstone_name);
			return;
		}

		Log(L"Setting hint text for tower tombstone " + tombstone_name);
		vector<wstring> text = Client::GetHintText(*info);
		TArray<FText>* textWindows = object->GetValuePtrByPropertyName<TArray<FText>>(L"textWindows");
		textWindows->Empty(text.size());
		for (const auto& wstr : text) {
			textWindows->Add(FText(wstr));
		}
	}

	void CreateMajorKeyHints(UObject* object) {
		if (GetCurrentMap(object) != GameData::Map::Tower) return;

		optional<GameData::MajorKeyInfo> info = GameData::GetMajorKeyInfo(object->GetName());
		if (!info) {
			return;
		}

		if (info->locations.size() == 0) {
			Log("No hints for key " + std::to_string(info->item_id));
			return;
		}

		if (info->found) {
			Log("Not creating hints for " + std::to_string(info->item_id) + ": key already found.");
			return;
		}

		Client::CreateMajorKeyHints(*info);
	}

	void Init() {
		switch (Settings::GetPopupsInitialState()) {
		case Settings::PopupsInitialState::ShowMuted:
			popups_muted = true;
			break;
		case Settings::PopupsInitialState::Hide:
			popups_hidden = true;
			break;
		}
	}

	void StartConnectHandshake(UObject* object) {
		lock_guard<mutex> guard(file_object_mutex);
		file_object = object;
	}

	bool IsInConnectHandshake() {
		lock_guard<mutex> guard(file_object_mutex);
		return file_object.has_value();
	}

	void UpdateConnectHandshakeStatus(wstring status, bool is_error) {
		lock_guard<mutex> guard(file_object_mutex);
		if (!file_object) return;

		struct UpdateConnectionStatus {
			FText new_text;
			bool is_error;
		};
		shared_ptr<void> params(new UpdateConnectionStatus{ FText(status), is_error });
		ExecuteBlueprintFunction(*file_object, L"UpdateConnectionStatus", params);
	}

    void FinishConnect(wstring zone, wstring player_start, wstring seed, wstring spawn_point, Version apworld_version) {
		lock_guard<mutex> guard(file_object_mutex);
		if (!file_object) return;

		struct FinishConnectInfo {
			FString zone;
			FString player_start;
			FString seed;
			FString spawn_point;
			int32_t major;
			int32_t minor;
			int32_t patch;
		};
		shared_ptr<void> params(new FinishConnectInfo{
			FString(zone.c_str()),
			FString(player_start.c_str()),
			FString(seed.c_str()),
			FString(spawn_point.c_str()),
			apworld_version[MAJOR],
			apworld_version[MINOR],
			apworld_version[PATCH],
		});
		ExecuteBlueprintFunction(*file_object, L"FinishConnect", params);
	}

	void FinishConnect() {
		lock_guard<mutex> guard(file_object_mutex);
		if (!file_object) return;
		ExecuteBlueprintFunction(*file_object, L"FinishConnect", nullptr);
	}

	void EndConnectHandshake() {
		lock_guard<mutex> guard(file_object_mutex);
		file_object = {};
	}

	void RecallBreaker() {
		UObject* player = UObjectGlobals::FindFirstOf(L"BP_PlayerGoatMain_C");
		if (!player) {
			Log("Couldn't find player object.", LogType::Error);
			return;
		}
		bool* obtainedAttack = player->GetValuePtrByPropertyName<bool>(L"obtainedAttack?");
		if (!*obtainedAttack) {
			Log("You haven't obtained Dream Breaker yet!", LogType::System);
			return;
		}
		Log("Recalling Dream Breaker.", LogType::System);
		ExecuteBlueprintFunction(player, L"recallWeapon", nullptr);
	}

	wstring VersionToWString(const Version& version) {
        return to_wstring(version[MAJOR]) + L"." + to_wstring(version[MINOR]) + L"." + to_wstring(version[PATCH]);
	}

	bool IsAPWorldVersionCompatible(const Version& apworld_version) {
		// major version must match
		if (client_version[MAJOR] != apworld_version[MAJOR])
			return false;

		// pre release, minor version must match
		if (client_version[MAJOR] == PRE_RELEASE)
			return client_version[MINOR] == apworld_version[MINOR];

		// post release, client supports previous minor versions
		return client_version[MINOR] >= apworld_version[MINOR];
	}

	void CheckVersionCompatibility(UnrealScriptFunctionCallableContext& context) {
		struct CheckVersionCompatibilityParams {
			int32_t major;
			int32_t minor;
			int32_t patch;
		};
		auto& params = context.GetParams<CheckVersionCompatibilityParams>();
		Engine::Version apworld_version = { params.major, params.minor, params.patch };
		auto* compatible = context.Context->GetValuePtrByPropertyName<bool>(L"VersionIsCompatible");
		*compatible = Engine::IsAPWorldVersionCompatible(apworld_version);
	}


	// Private functions
	namespace {
		void QueueItemSync() {
			lock_guard<mutex> guard(item_sync_mutex);
			if (!item_sync_queued) return;
			SyncHealthPieces();
			SyncSmallKeys();
			SyncMajorKeys();
			SyncAbilities();
			item_sync_queued = false;
		}

		void SyncHealthPieces() {
			shared_ptr<void> hp_params(new int(GameData::GetHealthPieces()));
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetHealthPieces", hp_params);
		}

		void SyncSmallKeys() {
			shared_ptr<void> small_key_params(new int(GameData::GetSmallKeys()));
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetSmallKeys", small_key_params);
		}

		void SyncMajorKeys() {
			struct MajorKeyInfo {
				TArray<bool> keys;
			};
			TArray<bool> ue_keys;
			bool* major_keys = GameData::GetMajorKeys();
			for (int i = 0; i < 5; i++) {
				ue_keys.Add(major_keys[i]);
			}
			shared_ptr<void> major_key_params(new MajorKeyInfo{ ue_keys });
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetMajorKeys", major_key_params);
		}

		void SyncAbilities() {
			struct AddUpgradeInfo {
				TArray<FName> names;
				TArray<int> counts;
				bool slidejump_disabled;
			};
			TArray<FName> ue_names;
			TArray<int> ue_counts;
			bool toggle = GameData::SlideJumpDisabled();

			for (const auto& [upgrade_name, upgrade_count] : GameData::GetUpgradeTable()) {
				FName new_name(upgrade_name);
				ue_names.Add(new_name);
				ue_counts.Add(upgrade_count);
			}
			shared_ptr<void> upgrade_params(new AddUpgradeInfo{ ue_names, ue_counts, toggle });
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetUpgrades", upgrade_params);
		}

		void SpawnCollectible(int64_t id, FVector position) {
			if (!Client::IsMissingLocation(id)) {
				Log(L"Collectible with id " + to_wstring(id) + L" was not spawned because it is not a missing location.");
				return;
			}
			if (spawned_collectibles.contains(id)) {
				Log(L"Collectible with id " + to_wstring(id) + L" has already been spawned");
				return;
			}
			Log(L"Spawning collectible with id " + to_wstring(id));
			struct CollectibleSpawnInfo {
				int64_t new_id;
				FVector new_position;
				int32_t classification;
			};
			shared_ptr<void> collectible_info(new CollectibleSpawnInfo{ id, position, GameData::GetClassification(id) });
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SpawnCollectible", collectible_info);
			spawned_collectibles.insert(id);
		}

		void SpawnInteractableAura(wstring name, GameData::Interactable interactable) {
			auto& [id, class_name] = interactable;
			if (!Client::IsMissingLocation(id)) {
				Log(L"Interactable aura with id " + to_wstring(id) + L" was not spawned because it is not a missing location");
				return;
			}
			UObject* object = UObjectGlobals::FindObject(class_name.c_str(), name.c_str());
			if (!object) {
				Log(L"No object found for interactable with id " + to_wstring(id));
				return;
			}
			Log(L"Spawning interactable aura with id " + to_wstring(id));
			struct InteractableAuraSpawnInfo {
				UObject* follow;
				int64_t interactableId;
				int32_t classification;
			};
			shared_ptr<void> interactable_aura_info(new InteractableAuraSpawnInfo{ object, id, GameData::GetClassification(id) });
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SpawnInteractableAura", interactable_aura_info);
		}

		void AddMessages(UObject* ap_object) {
			lock_guard<mutex> guard(messages_mutex);
			if (message_debounce_locked) return;

			bool* console_exists = ap_object->GetValuePtrByPropertyName<bool>(L"console_exists");
			if (!*console_exists) return;

			bool* console_initialized = ap_object->GetValuePtrByPropertyName<bool>(L"console_initialized");
			size_t print_num = *console_initialized ? queued_messages : messages.size();
			*console_initialized = true;
			if (print_num == 0) return;

			struct AddMessagesInfo {
				TArray<FText> markdown_messages;
				TArray<FText> plain_messages;
				bool show_console;
			};
			TArray<FText> ue_markdown_messages;
			TArray<FText> ue_plain_messages;
			size_t skip_num = messages.size() - print_num;
			size_t skipped = 0;
			for (const auto& [markdown, plain] : messages) {
				if (skipped < skip_num) {
					// TODO would it be better to just do random access?
					// messages has a max size of 100 so this probably isn't too bad
					skipped++;
					continue;
				}
				ue_markdown_messages.Add(FText(markdown));
				ue_plain_messages.Add(FText(plain));
			}
			bool show_console = queued_messages > 0;

			shared_ptr<void> params(new AddMessagesInfo{ ue_markdown_messages, ue_plain_messages, show_console });
			ExecuteBlueprintFunction(ap_object, L"AP_AddMessages", params);

			queued_messages = 0;
			Timer::RunTimerRealTime(message_debounce_delay, &message_debounce_locked);
		}

		void ShowQueuedPopup(UObject* ap_object) {
			lock_guard<mutex> guard(popups_mutex);
			if (popup_debounce_locked || !queued_popup) return;

			// don't show popup if the console hasn't been created yet, eg during the dungeon cutscene
			bool* console_exists = ap_object->GetValuePtrByPropertyName<bool>(L"console_exists");
			if (!*console_exists) return;

			if (holds_alternative<wstring>(*queued_popup)) {
				wstring popup_text = get<wstring>(*queued_popup);
				Log(popup_text, LogType::Popup);
				struct PrintMessageInfo {
					FText text;
					bool mute_sound;
				};
				FText new_text(popup_text);
				shared_ptr<void> params(new PrintMessageInfo{ new_text, popups_muted });
				ExecuteBlueprintFunction(ap_object, L"AP_PrintMessage", params);
			}
			else {
				ItemPopup popup = get<ItemPopup>(*queued_popup);
				Log(popup.preamble + popup.item + L" " + popup.info, LogType::Popup);
				struct PrintItemMessageInfo {
					FText preamble;
					FText item;
					FText info;
					bool mute_sound;
				};
				shared_ptr<void> params;
				if (Settings::GetPopupsSimplifyItemFont()) {
					params = std::make_shared<PrintItemMessageInfo>(
						FText(popup.preamble + popup.item),
						FText(L""),
						FText(popup.info),
						popups_muted);
				}
				else {
					params = std::make_shared<PrintItemMessageInfo>(
						FText(popup.preamble),
						FText(popup.item),
						FText(popup.info),
						popups_muted);
				}
				ExecuteBlueprintFunction(ap_object, L"AP_PrintItemMessage", params);
			}

			queued_popup = {};
			Timer::RunTimerRealTime(popup_debounce_delay, &popup_debounce_locked);
		}

		void CreateOverlay(UObject* ap_object) {
			struct CreateOverlayInfo {
				FText Version;
			};
			shared_ptr<void> params = std::make_shared<CreateOverlayInfo>(FText(client_version_text));
			ExecuteBlueprintFunction(ap_object, L"AP_CreateOverlay", params);
		}

		void VerifyGameVersion(GameData::Map map) {
			if (verified_version || !GameData::CanHaveTimeTrial(map)) return;

			int game_version = GameData::GetOptions().at("game_version");
			std::vector<UObject*> time_trials{};
			UObjectGlobals::FindAllOf(L"BP_TimeTrial_C", time_trials);
			bool time_trials_found = time_trials.size() != 0;
			if (game_version == GameData::MAP_PATCH && !time_trials_found) {
				Log("Game version map_patch was chosen in the player options, but it seems like you are playing on full gold. "
					"Switch to map patch for the intended experience.", LogType::Error);
			}
			else if (game_version == GameData::FULL_GOLD && time_trials_found) {
				Log("Game version full_gold was chosen in the player options, but it seems like you are playing on map patch. "
					"Switch to full gold for the intended experience.", LogType::Error);
			}
			else {
				Log("Verified that game version matches slot data.");
			}
			verified_version = true;
		}
	} // End private functions
}