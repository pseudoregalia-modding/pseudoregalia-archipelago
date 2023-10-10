#pragma once
#include "GameData.hpp"

namespace Pseudoregalia_AP {
    int GameData::GetHealthPieces() {
        return health_pieces;
    }

    int GameData::GetSmallKeys() {
        return small_keys;
    }

    bool* GameData::GetMajorKeys() {
        return major_keys;
    }
}