from BaseClasses import CollectionState, MultiWorld
from typing import Dict, Callable, Set
from worlds.generic.Rules import add_rule
from . import PseudoregaliaWorld


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


class PseudoregaliaRules:
    world: PseudoregaliaWorld
    player: int
    # TODO: Consider breaking these down a bit more

    region_rules: Dict[str, Dict[str, Set[Callable[[CollectionState], bool]]]]
    location_rules: Dict[str, Dict[str, Set[Callable[[CollectionState], bool]]]]

    def __init__(self, world: PseudoregaliaWorld) -> None:
        self.world = world
        self.player = world.player

        self.location_rules = {
            # Only Universal rules are always included, e.g. Normal rules are NOT added if the difficulty is set to Hard.
            # Care must be taken to ensure that relevant rules are not excluded from harder difficulties.
            "Castle - Platform In Main Halls": {
                UNIVERSAL: [
                    [lambda state: state.has("Sunsetter", self.player)],
                    [lambda state: state.has("Cling Gem", self.player)],
                ],
                NORMAL: [
                    [lambda state: self.get_kicks(state, self.player) >= 2],
                ],
                HARD: [
                    [lambda state: self.get_kicks(state, self.player) >= 1],
                ],
                EXPERT: [
                    [lambda state: self.get_kicks(state, self.player) >= 1],
                    [lambda state: state.has("Slide", self.player)],
                ],
                LUNATIC: [
                    [lambda state: self.get_kicks(state, self.player) >= 1],
                    [lambda state: state.has("Slide", self.player)],
                    [lambda state: self.can_bounce(state, self.player)],
                ]
            },
            "Castle - Corner Corridor": {
                UNIVERSAL: [
                    [lambda state: state.has("Cling Gem", self.player)],
                ],
                NORMAL: [
                    [lambda state: self.get_kicks(state, self.player) >= 4],
                ],
                HARD: [
                    [lambda state: self.get_kicks(state, self.player) >= 3],
                ],
                EXPERT: [
                    [lambda state: self.get_kicks(state, self.player) >= 3],
                    [lambda state: state.has("Slide", self.player) and self.get_kicks(state, self.player) >= 2],
                ],
                LUNATIC: [
                    [lambda state: self.get_kicks(state, self.player) >= 3],
                    [lambda state: state.has("Slide", self.player) and self.get_kicks(state, self.player) >= 1],
                ],
            }
        }

    def set_pseudoregalia_rules(self, multiworld, player, difficulty):
        for location in self.location_rules:
            for rule in location[UNIVERSAL]:
                add_rule(multiworld.get_location(location, player), rule, "or")
            match difficulty:
                case 0:
                    for rule in location[NORMAL]:
                        add_rule(multiworld.get_location(location, player), rule, "or")
                case 1:
                    for rule in location[HARD]:
                        add_rule(multiworld.get_location(location, player), rule, "or")
                case 2:
                    for rule in location[EXPERT]:
                        add_rule(multiworld.get_location(location, player), rule, "or")
                case 3:
                    for rule in location[LUNATIC]:
                        add_rule(multiworld.get_location(location, player), rule, "or")

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
