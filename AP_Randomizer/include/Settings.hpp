#pragma once

#include "APOptions.hpp"

namespace Settings {
    enum class PopupsInitialState {
        ShowWithSound,
        ShowMuted,
        Hide,
    };

    namespace Filters {
        enum class ItemSend {
            All,
            Relevant,
            None
        };
    }

    void Load();
    bool GetDeathLink();
    EPopupDisplay::Type GetPopupDisplay();
    bool GetPopupsSimplifyItemFont();
    Filters::ItemSend GetItemSendFilter();

    void Load_New(FF_APOptions*);
    void Update(FF_APOptions*);
}
