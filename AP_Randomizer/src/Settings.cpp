#pragma once

#include "Settings.hpp"

#include "Client.hpp"
#include "Engine.hpp"
#include "Logger.hpp"

namespace Settings {
	namespace {
		// settings, set to their defaults
		bool death_link = false;
		EPopupDisplay::Type popup_display = EPopupDisplay::Type::VisibleWithSound;
		bool popups_simplify_item_font = false;
		EConsoleMessageFilter::Type item_send_filter = EConsoleMessageFilter::Type::All;

		bool settings_loaded = false;
	}

	bool GetDeathLink() {
		return death_link;
	}

	EPopupDisplay::Type GetPopupDisplay() {
		return popup_display;
	}

	bool GetPopupsSimplifyItemFont() {
		return popups_simplify_item_font;
	}

	EConsoleMessageFilter::Type GetItemSendFilter() {
		return item_send_filter;
	}

	void Load(FF_APOptions* options) {
		if (settings_loaded) return;

		death_link = options->DeathLink;
		settings_loaded = true;
	}

	void Update(FF_APOptions* options) {
		bool updated = false;
		if (options->DeathLink != death_link) {
			death_link = options->DeathLink;
			Client::UpdateTags(death_link);
			updated = true;
		}
		if (options->ItemSendFilter != item_send_filter) {
			item_send_filter = options->ItemSendFilter.GetValue();
			updated = true;
		}
		if (options->PopupDisplay != popup_display) {
			popup_display = options->PopupDisplay.GetValue();
			Engine::UpdatePopupDisplay(popup_display);
			updated = true;
		}
		if (options->SimplifyItemPopupFont != popups_simplify_item_font) {
			popups_simplify_item_font = options->SimplifyItemPopupFont;
			updated = true;
		}
		if (updated) {
			Log(L"backend relevant settings updated");
		}
	}
}
