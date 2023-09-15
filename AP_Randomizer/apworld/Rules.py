from BaseClasses import CollectionState, MultiWorld
from typing import Dict, Callable

# Placed for better legibility since dream breaker is an exclusive requirement for breakable walls


def has_breaker(state: CollectionState, player: int) -> bool:
    return state.has(state, "Dream Breaker", player)


def can_bounce(state: CollectionState, player: int) -> bool:
    return state.has_all(state, {"Dream Breaker", "Ascendant Light"}, player)


def get_kicks(state: CollectionState, player: int) -> int:
    kicks: int = 0
    if (state.has(state, "Sun Greaves", player)):
        kicks += 3
    kicks += state.count(state, "Sun Greaves", player)
    return kicks


def has_small_keys(state: CollectionState, player: int) -> bool:
    return (state.count(state, "Small Key", player) >= 6)


def navigate_darkrooms(state: CollectionState, player: int) -> bool:
    return (state.has(state, "Dream Breaker", player) or state.has(state, "Ascendant Light", player))


def can_slidejump(state: CollectionState, player: int) -> bool:
    return (state.has_all(state, ["Slide", "Solar Wind"]))


location_rules: Dict(str, Callable[[CollectionState, int], bool]) = {
    "Dungeon - Slide": has_breaker,
    "Keep - Sunsetter": has_breaker,
    "Keep - Strikebreak": lambda state, player:
        (state.has("Slide") or state.has("Strikebreak"))
        and any(
            can_slidejump(state, player),
            get_kicks(state, player) >= 1,
            state.has("Cling Gem", player),
        ),
    "Library - Sun Greaves": lambda state, player:
        any(
            state.has("Slide", player) and has_breaker(state, player),
            state.has("Cling Gem", player) and has_breaker(state, player),
            get_kicks(state, player) >= 4,
            can_slidejump(state, player),
        ),
    "Theatre - Soul Cutter": lambda state, player: state.has("Strikebreak", player),
    "Bailey - Solar Wind": lambda state, player: state.has("Slide"),
    "Underbelly - Ascendant Light": lambda state, player:
        any(
            can_bounce(state, player),
            get_kicks(state, player) >= 3,
            state.has("Cling Gem", player),
            can_slidejump(state, player) and get_kicks > 0,
    ),
    "Tower - Cling Gem": get_kicks >= 3,
}
