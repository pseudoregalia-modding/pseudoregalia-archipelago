#pragma once

#include "APOptions.hpp"

namespace Settings {
    enum class ItemDisplay {
        Full,
        GenericNonPseudo,
        GenericAll,
    };

    enum class InteractableAuraDisplay {
        Classification,
        Generic,
        None,
    };

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
    ItemDisplay GetItemDisplay();
    InteractableAuraDisplay GetInteractableAuraDisplay();
    bool GetDeathLink();
    PopupsInitialState GetPopupsInitialState();
    bool GetPopupsSimplifyItemFont();
    Filters::ItemSend GetItemSendFilter();

    void Load_New(FF_APOptions*);
    void Update(FF_APOptions*);
}
