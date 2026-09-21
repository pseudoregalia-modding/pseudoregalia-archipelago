#pragma once

#include "Unreal/CoreUObject/UObject/Class.hpp"

namespace Settings {
    namespace EConsoleMessageFilter {
        enum Type {
            All = 0,
            Relevant = 1,
            None = 2,
            EConsoleMessageFilter_MAX = 3,
        };
    }

    namespace EPseudoItemDisplay {
        enum Type {
            Vanilla = 0,
            Classification = 1,
            Basic = 2,
            EPseudoItemDisplay_MAX = 3,
        };
    }

    namespace EOffWorldItemDisplay {
        enum Type {
            Classification = 0,
            Basic = 1,
            EOffWorldItemDisplay_MAX = 2,
        };
    }

    namespace EInteractableDisplay {
        enum Type {
            Classification = 0,
            Basic = 1,
            None = 2,
            EInteractableDisplay_MAX = 3,
        };
    }

    namespace EPopupDisplay {
        enum Type {
            VisibleWithSound = 0,
            VisibleMuted = 1,
            Hidden = 2,
            EPopupDisplay_MAX = 3,
        };
    }

    struct FF_APOptions
    {
        bool DeathLink;
        RC::Unreal::TEnumAsByte<EConsoleMessageFilter::Type> ItemSendFilter;
        RC::Unreal::TEnumAsByte<EPseudoItemDisplay::Type> PseudoItemDisplay;
        RC::Unreal::TEnumAsByte<EOffWorldItemDisplay::Type> OffWorldItemDisplay;
        RC::Unreal::TEnumAsByte<EInteractableDisplay::Type> InteractableItemDisplay;
        RC::Unreal::TEnumAsByte<EPopupDisplay::Type> PopupDisplay;
        bool SimplifyItemPopupFont;
    };

    void Load(FF_APOptions*);
    void Update(FF_APOptions*);

    bool GetDeathLink();
    EPopupDisplay::Type GetPopupDisplay();
    bool GetPopupsSimplifyItemFont();
    EConsoleMessageFilter::Type GetItemSendFilter();
}
