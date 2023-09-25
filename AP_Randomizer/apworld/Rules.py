from BaseClasses import CollectionState, MultiWorld
from typing import Dict, Callable

# Placed for better legibility since dream breaker is an exclusive requirement for breakable walls


def has_breaker(state: CollectionState, player: int) -> bool:
    return state.has("Dream Breaker", player)


def can_bounce(state: CollectionState, player: int) -> bool:
    return state.has_all({"Dream Breaker", "Ascendant Light"}, player)


def get_kicks(state: CollectionState, player: int) -> int:
    kicks: int = 0
    if (state.has("Sun Greaves", player)):
        kicks += 3
    kicks += state.count("Heliacal Power", player)
    return kicks


def has_small_keys(state: CollectionState, player: int) -> bool:
    return (state.count("Small Key", player) >= 6)


def navigate_darkrooms(state: CollectionState, player: int) -> bool:
    return (state.has("Dream Breaker", player) or state.has("Ascendant Light", player))


def can_slidejump(state: CollectionState, player: int) -> bool:
    return (state.has_all(["Slide", "Solar Wind"], player))


def can_strikebreak(state: CollectionState, player: int) -> bool:
    return (state.has_all(["Dream Breaker", "Strikebreak"], player))


def can_soulcutter(state: CollectionState, player: int) -> bool:
    return (state.has_all(["Dream Breaker", "Strikebreak", "Soul Cutter"], player))
