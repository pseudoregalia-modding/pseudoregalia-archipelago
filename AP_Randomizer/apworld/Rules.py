from BaseClasses import CollectionState, MultiWorld
from typing import Dict, Callable


UNIVERSAL = 0
NORMAL = 1
HARD = 2
EXPERT = 3
LUNATIC = 4


BREAKER = 101
INDIGNATION = 102
GREAVES = 103
SLIDE = 104
SOLAR_WIND = 105
SUNSETTER = 106
STRIKEBREAK = 107
CLING_GEM = 108
LIGHT = 109
SOUL_CUTTER = 110
AIR_KICK = 111
HEALTH_PIP = 112
SMALL_KEY = 113
MAJOR_KEY = 114
ATTACK = 201
SIDEFLIP = 202
SUPER = 203
ULTRA = 204
REVKICK = 205
CLING_CLIMB = 206
KICK_CANCEL = 207
DARKROOMS = 208
OBSCURE = 301
PRECISE = 302
VERY_PRECISE = 303
EXTREMELY_PRECISE = 304


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
