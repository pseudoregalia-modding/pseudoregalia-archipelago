#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include "Logger.hpp"
#include "Settings.hpp"
#include "toml++/toml.hpp"

namespace Settings {
	using std::optional;
	using std::string;
	using std::ifstream;
	using std::unordered_map;
	using std::pair;
	using std::list;

	namespace {
		// if you run from the executable directory
		const string settings_filename1 = "Mods/AP_Randomizer/settings.toml";
		// if you run from the game directory
		const string settings_filename2 = "pseudoregalia/Binaries/Win64/Mods/AP_Randomizer/settings.toml";

		// settings, set to their defaults
		ItemDisplay item_display = ItemDisplay::Full;
		InteractableAuraDisplay interactable_aura_display = InteractableAuraDisplay::Classification;
		bool death_link = false;
		PopupsInitialState popups_initial_state = PopupsInitialState::ShowWithSound;
		bool popups_simplify_item_font = false;
		Filters::ItemSend item_send_filter = Filters::ItemSend::All;

		template<class E> void ParseSetting(E&, toml::table, string, unordered_map<string, E>);
		void ParseSetting(bool&, toml::table, string);
	}

	void Load() {
		ifstream settings_file(settings_filename1);
		if (!settings_file.good()) {
			settings_file = ifstream(settings_filename2);
			if (!settings_file.good()) {
				Log("Settings file not found, using default settings");
				return;
			}
		}

		toml::table settings_table;
		try {
			settings_table = toml::parse(settings_file);
		}
		catch (const toml::parse_error& err) {
			Log("Failed to parse settings: " + string(err.description()));
			Log("Using default settings");
			return;
		}

		Log("Loading settings");
		ParseSetting(
			item_display,
			settings_table,
			"settings.item_display",
			unordered_map<string, ItemDisplay>{
				{ "full", ItemDisplay::Full },
				{ "generic_non_pseudo", ItemDisplay::GenericNonPseudo },
				{ "generic_all", ItemDisplay::GenericAll },
			});
		ParseSetting(
			interactable_aura_display,
			settings_table,
			"settings.interactable_aura_display",
			unordered_map<string, InteractableAuraDisplay>{
				{ "classification", InteractableAuraDisplay::Classification },
				{ "generic", InteractableAuraDisplay::Generic },
				{ "none", InteractableAuraDisplay::None },
			});
		ParseSetting(death_link, settings_table, "settings.death_link");
		ParseSetting(
			popups_initial_state,
			settings_table,
			"settings.popups.initial_state",
			unordered_map<string, PopupsInitialState>{
				{ "show_with_sound", PopupsInitialState::ShowWithSound },
				{ "show_muted", PopupsInitialState::ShowMuted },
				{ "hide", PopupsInitialState::Hide },
			});
		ParseSetting(popups_simplify_item_font, settings_table, "settings.popups.simplify_item_font");
		ParseSetting(
			item_send_filter,
			settings_table,
			"settings.console.filters.item_send",
			unordered_map<string, Filters::ItemSend> {
				{ "all", Filters::ItemSend::All },
				{ "relevant", Filters::ItemSend::Relevant },
				{ "none", Filters::ItemSend::None },
			});
	}

	ItemDisplay GetItemDisplay() {
		return item_display;
	}

	InteractableAuraDisplay GetInteractableAuraDisplay() {
		return interactable_aura_display;
	}

	bool GetDeathLink() {
		return death_link;
	}

	PopupsInitialState GetPopupsInitialState() {
		return popups_initial_state;
	}

	bool GetPopupsSimplifyItemFont() {
		return popups_simplify_item_font;
	}

	Filters::ItemSend GetItemSendFilter() {
		return item_send_filter;
	}

	namespace {
		template<class E> void ParseSetting(
			E& setting,
			toml::table settings_table,
			string setting_path,
			unordered_map<string, E> option_map
		) {
			optional<string> option = settings_table.at_path(setting_path).value<string>();
			if (option) {
				if (option_map.contains(*option)) {
					Log(setting_path + " = " + *option);
					setting = option_map.at(*option);
					return;
				}

				Log(setting_path + " = default (unknown option " + *option + ")");
				return;
			}

			Log(setting_path + " = default (setting missing or not a string)");
			return;
		}

		void ParseSetting(bool& setting, toml::table settings_table, string setting_path) {
			optional<bool> option = settings_table.at_path(setting_path).value<bool>();
			if (option) {
				string option_string = *option ? "true" : "false";
				Log(setting_path + " = " + option_string);
				setting = *option;
				return;
			}
			Log(setting_path + " = default (setting missing or not a bool)");
		}
	}
}
