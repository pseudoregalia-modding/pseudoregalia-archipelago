#pragma once

#include "APOptions.hpp"

namespace Settings {
    void Load(FF_APOptions*);
    void Update(FF_APOptions*);

    bool GetDeathLink();
    EPopupDisplay::Type GetPopupDisplay();
    bool GetPopupsSimplifyItemFont();
    EConsoleMessageFilter::Type GetItemSendFilter();
}
