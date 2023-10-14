from BaseClasses import CollectionState, MultiWorld
from typing import Dict, Callable, Set
from worlds.generic.Rules import add_rule


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
    player: int
    # TODO: Consider breaking these down a bit more

    region_rules: Dict[str, Dict[str, Set[Callable[[CollectionState], bool]]]]
    location_rules: Dict[str, Dict[str, Set[Callable[[CollectionState], bool]]]]

    def __init__(self, player) -> None:
        self.player = player

        self.region_rules = {
            "Dungeon Mirror": {
                "Dungeon Strong Eyes": {
                    UNIVERSAL: [
                        [lambda state: state.has("Slide", self.player)
                         and self.has_breaker(state)],
                    ],
                },
                "Underbelly Main": {
                    UNIVERSAL: [
                        [lambda state: self.has_breaker(state)],
                    ],
                },
                "Theatre Main": {
                    UNIVERSAL: [
                        [lambda state: state.has("Cling Gem", player)
                         and self.get_kicks(state, 3)],
                        [lambda state: state.has("Cling Gem", player)
                         and self.can_slidejump(state)],
                    ],
                },
            },
            "Dungeon Strong Eyes": {
                "Castle Sansa": {
                    UNIVERSAL: [
                        [lambda state: self.has_small_keys(state)],
                    ],
                },
            },
            "Castle Sansa": {
                "Library Main": {
                    UNIVERSAL: [
                        [lambda state: self.has_breaker(state)],
                    ],
                },
                "Keep Main": {},
                "Empty Bailey": {},
                "Theatre Pillar": {
                    UNIVERSAL: [
                        [lambda state: self.get_kicks(state, 1)],
                        [lambda state: state.has("Cling Gem", self.player)],
                        [lambda state: state.has("Sunsetter", self.player)],
                    ],
                },
                "Theatre Main": {
                    UNIVERSAL: [
                        [lambda state: state.has("Cling Gem", self.player)],
                        [lambda state: self.can_slidejump(state)
                         and self.get_kicks(state, 4)],
                    ],
                },
            },
            "Library Main": {
                "Library Locked": {
                    UNIVERSAL: [
                        [lambda state: self.has_small_keys(state)],
                    ],
                },
            },
            "Library Locked": {},
            "Keep Main": {
                "Keep Sunsetter": {
                    UNIVERSAL: [
                        [lambda state: state.has("Cling Gem", self.player)],
                        [lambda state: self.has_small_keys(state)],
                        [lambda state: self.get_kicks(state, 3)],
                    ],
                },
                "Underbelly Hole": {
                    UNIVERSAL: [
                        [lambda state: state.has("Sunsetter", self.player)],
                        [lambda state: self.get_kicks(state, 1)],
                    ],
                },
                "Theatre Main": {
                    UNIVERSAL: [
                        [lambda state: state.has("Cling Gem", self.player)
                         and self.get_kicks(state, 3)],
                        [lambda state: state.has("Cling Gem", self.player)
                         and self.can_slidejump(state)],
                    ],
                },
            },
            "Keep Sunsetter": {},
            "Empty Bailey": {
                "Castle Sansa": {},
                "Theatre Pillar": {},
                "Tower Remains": {
                    UNIVERSAL: [
                        [lambda state: state.has("Cling Gem", self.player)],
                        [lambda state: state.has_all(["Slide", "Sunsetter"], self.player)],
                        [lambda state: self.get_kicks(state, 1)],
                    ],
                },
            },
            "Tower Remains": {
                "Underbelly Main": {
                    UNIVERSAL: [
                        [lambda state: state.has("Sunsetter", self.player)],
                    ],
                },
                "The Great Door": {
                    UNIVERSAL: [
                        [lambda state: state.has("Cling Gem", self.player)
                         and self.get_kicks(state, 3)],
                    ],
                },
            },
            "Underbelly Main": {
                "Empty Bailey": {
                    UNIVERSAL: [
                        [lambda state: state.has("Sunsetter")],
                        [lambda state: self.has_breaker(state)],
                    ],
                },
            },
            "Underbelly Hole": {
                "Underbelly Main": {
                    UNIVERSAL: [
                        [lambda state: state.has("Sunsetter")],
                    ],
                },
            },
            "Theatre Main": {
                "Keep Main": {
                    UNIVERSAL: [
                        [lambda state: state.has("Cling Gem")],
                    ],
                },
            },
            "Theatre Pillar": {
                "Theatre Main": {
                    UNIVERSAL: [
                        [lambda state: state.has_all(["Sunsetter", "Cling Gem"], self.player)],
                        [lambda state: state.has("Sunsetter", self.player)
                         and self.get_kicks(state, 4)],
                    ],
                },
            },
        }

        self.location_rules = {
            # Only Universal rules are always included, e.g. Normal rules are NOT added if the difficulty is set to Hard.
            # Care must be taken to ensure that relevant rules are not excluded from harder difficulties.
            "Castle - Platform In Main Halls": {
                UNIVERSAL: [
                    [lambda state: state.has("Sunsetter", self.player)],
                    [lambda state: state.has("Cling Gem", self.player)],
                ],
                NORMAL: [
                    [lambda state: self.get_kicks(state, 2)],
                ],
                HARD: [
                    [lambda state: self.get_kicks(state, 1)],
                ],
                EXPERT: [
                    [lambda state: self.get_kicks(state, 1)],
                    [lambda state: state.has("Slide", self.player)],
                ],
                LUNATIC: [
                    [lambda state: self.get_kicks(state, 1)],
                    [lambda state: state.has("Slide", self.player)],
                    [lambda state: self.can_bounce(state, self.player)],
                ]
            },
            "Castle - Corner Corridor": {
                UNIVERSAL: [
                    [lambda state: state.has("Cling Gem", self.player)],
                ],
                NORMAL: [
                    [lambda state: self.get_kicks(state, 4)],
                ],
                HARD: [
                    [lambda state: self.get_kicks(state, 3)],
                ],
                EXPERT: [
                    [lambda state: self.get_kicks(state, 3)],
                    [lambda state: state.has("Slide", self.player) and self.get_kicks(state, 2)],
                ],
                LUNATIC: [
                    [lambda state: self.get_kicks(state, 3)],
                    [lambda state: state.has("Slide", self.player) and self.get_kicks(state, 1)],
                ],
            }
        }

    def set_pseudoregalia_rules(self, multiworld, difficulty):
        for region in self.region_rules:
            for exit in region:
                for rule in exit[UNIVERSAL]:
                    add_rule(multiworld.get_entrance(exit, self.player), rule, "or")
                for rule in exit[difficulty]:
                    add_rule(multiworld.get_entrance(exit, self.player), rule, "or")

        for location in self.location_rules:
            for rule in location[UNIVERSAL]:
                add_rule(multiworld.get_location(location, self.player), rule, "or")
            for rule in exit[difficulty]:
                add_rule(multiworld.get_location(location, self.player), rule, "or")

    # Placed for better legibility since dream breaker is an exclusive requirement for breakable walls

    def has_breaker(self, state) -> bool:
        return state.has("Dream Breaker", self.player)

    def can_bounce(self, state) -> bool:
        return state.has_all({"Dream Breaker", "Ascendant Light"}, self.player)

    def get_kicks(self, state, count: int) -> bool:
        kicks: int = 0
        if (state.has("Sun Greaves", self.player)):
            kicks += 3
        kicks += state.count("Heliacal Power", self.player)
        return kicks >= count

    def has_small_keys(self, state) -> bool:
        return (state.count("Small Key", self.player) >= 6)

    def navigate_darkrooms(self, state) -> bool:
        return (state.has("Dream Breaker", self.player) or state.has("Ascendant Light", self.player))

    def can_slidejump(self, state) -> bool:
        return (state.has_all(["Slide", "Solar Wind"], self.player))

    def can_strikebreak(self, state) -> bool:
        return (state.has_all(["Dream Breaker", "Strikebreak"], self.player))

    def can_soulcutter(self, state) -> bool:
        return (state.has_all(["Dream Breaker", "Strikebreak", "Soul Cutter"], self.player))
