from BaseClasses import CollectionState, MultiWorld
from typing import Dict, Callable, Set
from worlds.generic.Rules import add_rule, set_rule
from .constants.DIFFICULTIES import UNIVERSAL, NORMAL, HARD, EXPERT, LUNATIC


class PseudoregaliaRules:
    player: int
    # TODO: Consider breaking these down a bit more

    region_rules: Dict[str, Dict[str, Dict[int, Set[Callable[[CollectionState], bool]]]]]
    location_rules: Dict[str, Dict[str, Set[Callable[[CollectionState], bool]]]]

    def __init__(self, player) -> None:
        self.player = player

        self.region_rules = {
            # TODO: Consider drawing the inner exits out to remove a level of nesting,
            # e.g. "Dungeon Mirror -> Dungeon Strong Eyes"
            "Dungeon Mirror": {
                "Dungeon Strong Eyes": {
                    UNIVERSAL: [
                        lambda state: self.has_slide(state) and self.has_breaker(state),
                    ],
                },
                "Underbelly Main": {
                    UNIVERSAL: [
                        lambda state: self.has_breaker(state),
                    ],
                },
                "Theatre Main": {
                    UNIVERSAL: [
                        lambda state: self.has_gem(state) and self.get_kicks(state, 3),
                        lambda state: self.has_gem(state) and self.can_slidejump(state),
                    ],
                },
            },
            "Dungeon Strong Eyes": {
                "Castle Sansa": {
                    UNIVERSAL: [
                        lambda state: self.has_small_keys(state),
                    ],
                },
            },
            "Castle Sansa": {
                "Library Main": {
                    UNIVERSAL: [
                        lambda state: self.has_breaker(state),
                    ],
                },
                "Keep Main": {},
                "Empty Bailey": {},
                "Theatre Pillar": {
                    UNIVERSAL: [
                    ],
                    NORMAL: [
                        lambda state: self.has_gem(state) and self.has_plunge(state),
                        lambda state: self.has_plunge(state) and self.get_kicks(state, 1),
                        lambda state: self.has_gem(state) and self.get_kicks(state, 1),
                        lambda state: self.get_kicks(state, 2),
                    ],
                    HARD: [
                        self.has_gem,
                        self.has_plunge,
                        lambda state: self.get_kicks(state, 1),
                    ],
                    EXPERT: [
                        self.has_gem,
                        self.has_plunge,
                        self.has_slide,
                        lambda state: self.get_kicks(state, 1),
                    ],
                    LUNATIC: [
                        self.has_gem,
                        self.has_plunge,
                        self.has_slide,
                        lambda state: self.get_kicks(state, 1),
                    ],
                },
                "Theatre Main": {
                    UNIVERSAL: [
                        self.has_gem,
                        lambda state: self.can_slidejump(state) and self.get_kicks(state, 4),
                    ],
                },
            },
            "Library Main": {
                "Library Locked": {
                    UNIVERSAL: [
                        lambda state: self.has_small_keys(state),
                    ],
                },
            },
            "Library Locked": {},
            "Keep Main": {
                "Keep Sunsetter": {
                    UNIVERSAL: [
                        self.has_gem,
                        lambda state: self.has_small_keys(state),
                        lambda state: self.get_kicks(state, 3),
                    ],
                },
                "Underbelly Hole": {
                    UNIVERSAL: [
                        self.has_plunge,
                        lambda state: self.get_kicks(state, 1),
                    ],
                },
                "Theatre Main": {
                    UNIVERSAL: [
                        lambda state: self.has_gem(state) and self.get_kicks(state, 3),
                        lambda state: self.has_gem(state) and self.can_slidejump(state),
                    ],
                },
            },
            "Keep Sunsetter": {},
            "Empty Bailey": {
                "Castle Sansa": {},
                "Theatre Pillar": {},
                "Tower Remains": {
                    UNIVERSAL: [
                        self.has_gem,
                        lambda state: state.has_all(["Slide", "Sunsetter"], self.player),
                        lambda state: self.get_kicks(state, 1),
                    ],
                },
            },
            "Tower Remains": {
                "Underbelly Main": {
                    UNIVERSAL: [
                        self.has_plunge,
                    ],
                },
                "The Great Door": {
                    UNIVERSAL: [
                        lambda state: self.has_gem(state) and self.get_kicks(state, 3),
                    ],
                },
            },
            "Underbelly Main": {
                "Empty Bailey": {
                    UNIVERSAL: [
                        self.has_plunge,
                        self.has_breaker,
                    ],
                },
            },
            "Underbelly Hole": {
                "Underbelly Main": {
                    UNIVERSAL: [
                        self.has_plunge,
                    ],
                },
            },
            "Theatre Main": {
                "Keep Main": {
                    UNIVERSAL: [
                        self.has_gem,
                    ],
                },
            },
            "Theatre Pillar": {
                "Theatre Main": {
                    UNIVERSAL: [
                        lambda state: state.has_all(["Sunsetter", "Cling Gem"], self.player),
                        lambda state: self.has_plunge(state) and self.get_kicks(state, 4),
                    ],
                },
            },
        }

        self.location_rules = {
            # Only Universal rules are always included, e.g. Normal rules are NOT added if the difficulty is set to Hard.
            # Care must be taken to ensure that relevant rules are not excluded from harder difficulties.
            # TODO: look into adding a 'REQUIRED' block, e.g. for an item behind a strikebreak wall that requires items to get to
            # TODO: look into turning each rule into a set which i can construct combined rules out of
            # TODO: honestly consider trying raw sets of items rather than lambdas with helper functions
            "Dungeon - Dream Breaker": {},
            "Dungeon - Slide": {
                UNIVERSAL: [
                    self.has_breaker
                ],
            },
            "Dungeon - Dark Orbs": {
                UNIVERSAL: [
                    self.has_gem,
                    lambda state: self.can_bounce(state) and self.get_kicks(state, 3),
                ],
            },
            "Dungeon - Rafters": {
                UNIVERSAL: [
                    self.has_gem,
                    lambda state: self.get_kicks(state, 3) and self.can_slidejump(state),
                    lambda state: self.get_kicks(state, 3) and self.has_plunge(state),
                ],
            },
            "Dungeon - Strong Eyes": {
                UNIVERSAL: [
                    self.has_breaker,
                ],
            },
            "Dungeon - Alcove Near Mirror": {},
            "Dungeon - Past Poles": {
                UNIVERSAL: [
                    self.has_gem,
                    lambda state: self.get_kicks(state, 3),
                ],
            },
            "Castle - Indignation": {},
            "Castle - Floater In Courtyard": {
                UNIVERSAL: [
                    lambda state: self.get_kicks(state, 4),
                    lambda state: state.has_all(["Cling Gem", "Sunsetter"], self.player),
                    lambda state: state.has("Cling Gem", self.player) and self.get_kicks(state, 1),
                    lambda state: state.has("Cling Gem", self.player) and self.can_slidejump(state),
                    lambda state: self.can_bounce(state) and self.get_kicks(state, 1),
                    lambda state: self.can_bounce(state) and state.has("Sunsetter", self.player),
                ],
            },
            "Castle - High Climb From Courtyard": {
                UNIVERSAL: [
                    self.has_gem,
                    lambda state: self.can_slidejump(state) and self.get_kicks(state, 1)
                ],
            },
            "Castle - Locked Door": {
                UNIVERSAL: [
                    self.has_small_keys
                ],
            },
            "Castle - Near Theatre Front": {
                UNIVERSAL: [
                    lambda state: self.has_gem(state) and self.get_kicks(state, 3),
                ],
            },
            "Castle - Platform In Main Halls": {
                UNIVERSAL: [
                    self.has_plunge,
                    self.has_gem,
                ],
                NORMAL: [
                    lambda state: self.get_kicks(state, 2),
                ],
                HARD: [
                    lambda state: self.get_kicks(state, 1),
                ],
                EXPERT: [
                    lambda state: self.get_kicks(state, 1),
                    self.has_slide,
                ],
                LUNATIC: [
                    lambda state: self.get_kicks(state, 1),
                    self.has_slide,
                    self.can_bounce,
                ],
            },
            "Castle - Tall Room Near Wheel Crawlers": {
                UNIVERSAL: [
                ],
                NORMAL: [
                    lambda state: self.has_gem(state) and self.has_plunge(state),
                    lambda state: self.has_gem(state) and self.get_kicks(state, 1),
                    lambda state: self.get_kicks(state, 2),
                ],
                HARD: [
                    self.has_gem,
                    lambda state: self.get_kicks(state, 1),
                    lambda state: self.can_slidejump(state) and self.has_plunge(state),  # Obscure
                ],
                EXPERT: [
                    self.has_gem,
                    lambda state: self.get_kicks(state, 1),
                    self.has_slide,
                ],
                LUNATIC: [
                    self.has_gem,
                    lambda state: self.get_kicks(state, 1),
                    self.has_slide,
                ],
            },
            "Castle - Alcove Near Dungeon": {
                UNIVERSAL: [
                ],
                NORMAL: [
                    lambda state: self.has_gem(state) and self.has_plunge(state),
                    lambda state: self.has_plunge(state) and self.get_kicks(state, 1),
                    lambda state: self.has_gem(state) and self.get_kicks(state, 1),
                    lambda state: self.get_kicks(state, 2),
                ],
                HARD: [
                    self.has_gem,
                    self.has_plunge,
                    lambda state: self.get_kicks(state, 1),
                ],
                EXPERT: [
                    self.has_gem,
                    self.has_plunge,
                    lambda state: self.get_kicks(state, 1),
                    self.has_slide,
                ],
                LUNATIC: [
                    self.has_gem,
                    self.has_plunge,
                    lambda state: self.get_kicks(state, 1),
                    self.has_slide,
                ],
            },
            "Castle - Alcove Near Scythe Corridor": {
                UNIVERSAL: [
                    lambda state: self.can_slidejump(state) and self.get_kicks(state, 4),
                    self.has_gem,
                ]
            },
            "Castle - Balcony": {
                UNIVERSAL: [
                    lambda state: self.get_kicks(state, 4),
                    self.has_gem,
                    lambda state: self.can_slidejump(state) and self.get_kicks(state, 1),
                ]
            },
            "Castle - Corner Corridor": {
                UNIVERSAL: [
                    self.has_gem,
                    lambda state: self.get_kicks(state, 4),
                ]
            },
            "Castle - Wheel Crawlers": {
                UNIVERSAL: [
                    self.can_bounce,
                    self.has_gem,
                ],
                NORMAL: [
                    lambda state: self.get_kicks(state, 2),
                    lambda state: self.get_kicks(state, 1)
                    and (self.has_gem(state) or self.can_slidejump(state)),
                ],
                HARD: [
                    lambda state: self.get_kicks(state, 1),
                    lambda state: self.can_slidejump(state) and self.has_plunge(state),
                ],
                EXPERT: [
                    lambda state: self.get_kicks(state, 1),
                    self.has_slide,
                    self.has_plunge,  # Obscure
                ],
                LUNATIC: [
                    lambda state: self.get_kicks(state, 1),
                    self.has_slide,
                    self.has_plunge,  # Obscure
                ],
            },
            "Library - Sun Greaves": {
                UNIVERSAL: [
                    self.can_slidejump,
                    lambda state: self.has_slide(state) and self.has_breaker(state),
                    lambda state: self.has_gem(state) and self.has_breaker(state),
                    lambda state: self.get_kicks(state, 4),
                ],
            },
            "Library - Locked Door Left": {
                UNIVERSAL: [
                    lambda state: self.get_kicks(state, 3),
                    self.has_gem,
                ]
            },
            "Library - Locked Door Across": {
                UNIVERSAL: [
                    self.can_slidejump,
                    lambda state: self.get_kicks(state, 1),
                    self.has_gem,
                ],
            },
            "Library - Upper Back": {
                UNIVERSAL: [
                    lambda state: self.get_kicks(state, 3),
                    self.has_gem,
                ]
            },
            "Keep - Strikebreak": {
                UNIVERSAL: [
                    lambda state: (self.has_slide(state) or self.can_strikebreak(state))
                    and (
                        self.can_slidejump(state)
                        or self.get_kicks(state, 1)
                        or self.has_gem(state),
                    )
                ],
            },
            "Keep - Sunsetter": {
                UNIVERSAL: [
                    self.has_breaker
                ],
            },
            "Keep - Near Theatre": {
                UNIVERSAL: [
                    self.has_plunge,
                    self.has_gem,
                    lambda state: self.get_kicks(state, 1),
                ],
            },
            "Keep - Levers Room": {
                UNIVERSAL: [
                    self.has_breaker
                ],
            },
            "Keep - Alcove Near Locked Door": {
                UNIVERSAL: [
                    self.can_slidejump,
                    lambda state: self.get_kicks(state, 3),
                    self.has_plunge,
                ]
            },
            "Keep - Major Key": {
                UNIVERSAL: [
                    lambda state: self.can_bounce(state) and state.has_all(["Cling Gem, Sunsetter"], self.player),
                    lambda state: self.has_gem(state) and self.can_bounce(state) and self.get_kicks(state, 3),
                ],
            },
            "Bailey - Solar Wind": {
                UNIVERSAL: [
                    self.has_slide,
                ],
            },
            "Bailey - Cheese Bell": {
                UNIVERSAL: [
                    lambda state: self.can_slidejump(state) and self.get_kicks(state, 1) and self.has_plunge(state),
                    lambda state: self.can_slidejump(state) and self.has_gem(state),
                    lambda state: self.get_kicks(state, 3) and self.has_plunge(state),
                ],
            },
            "Bailey - Inside Building": {
                UNIVERSAL: [
                    self.has_slide,
                ],
            },
            "Bailey - Center Steeple": {
                UNIVERSAL: [
                    lambda state: self.get_kicks(state, 3),
                    lambda state: state.has_all(["Sunsetter", "Slide"], self.player),
                ]
            },
            "Bailey - Major Key": {
                UNIVERSAL: [
                    self.has_plunge,
                    self.has_gem,
                    lambda state: self.get_kicks(state, 3),
                ],
            },
            "Theatre - Soul Cutter": {
                UNIVERSAL: [
                    self.can_strikebreak
                ],
            },
            "Theatre - Corner Beam": {
                UNIVERSAL: [
                    lambda state: self.has_gem(state) and self.get_kicks(state, 3),
                    lambda state: self.has_gem(state) and self.can_slidejump(state),
                    lambda state: self.get_kicks(state, 3) and self.can_slidejump(state),
                ],
            },
            "Theatre - Locked Door": {
                UNIVERSAL: [
                    self.has_small_keys,
                    lambda state: self.get_kicks(state, 3),
                    self.has_gem,
                ],
            },
            "Theatre - Back Of Auditorium": {
                UNIVERSAL: [
                    lambda state: self.get_kicks(state, 3),
                    self.has_gem,
                ]
            },
            "Theatre - Murderous Goat": {},
            "Theatre - Major Key": {
                UNIVERSAL: [
                    lambda state: (self.can_soulcutter(state) and self.has_gem(state))
                    and (
                        self.can_slidejump(state)
                        or self.get_kicks(state, 1)
                    )
                ],
            },
            "Underbelly - Ascendant Light": {},
            "Underbelly - Locked Door": {
                UNIVERSAL: [
                    lambda state: (self.has_small_keys(state) and self.has_slide(state))
                    and (
                        self.get_kicks(state, 3)
                        or self.has_plunge(state)
                    )
                ],
            },
            "Underbelly - Strikebreak Wall": {
                UNIVERSAL: [
                    lambda state: (self.can_strikebreak and self.can_bounce(state))
                    and (
                        self.can_slidejump(state)
                        or self.get_kicks(state, 1)
                        or self.has_plunge(state)
                    )
                ],
            },
            "Underbelly - Main Room": {
                UNIVERSAL: [
                    self.can_slidejump,
                    self.has_plunge,
                ],
            },
            "Underbelly - Alcove Near Light": {},
            "Underbelly - Building Near Little Guy": {
                UNIVERSAL: [
                    lambda state: self.get_kicks(state, 3),
                    self.has_plunge,
                ]
            },
            "Underbelly - Rafters Near Keep": {
                UNIVERSAL: [
                    lambda state: self.get_kicks(state, 3),
                    lambda state: self.get_kicks(state, 1) and self.has_plunge(state),
                ]
            },
            "Underbelly - Major Key": {
                UNIVERSAL: [
                    lambda state: self.has_plunge(state)
                    and (
                        self.can_soulcutter(state) and self.can_bounce(state)
                        or self.can_soulcutter(state) and self.has_gem(state)
                        or self.can_slidejump(state) and self.get_kicks(state, 3)
                    )
                ],
            },
            "Tower - Cling Gem": {
                UNIVERSAL: [
                    lambda state: self.get_kicks(state, 3)
                ],
            },
            "Tower - Major Key": {},
        }

    def set_pseudoregalia_rules(self, multiworld, difficulty):
        # TODO: test performance of just iterating through region_rules as sets
        # TODO: consider experimenting with assigning rules on region/location creation
        # TODO: this method kind of just sucks honestly
        for region_name, exit_list in self.region_rules.items():
            for exit_name, rulesets in exit_list.items():
                entrance = multiworld.get_entrance(f"{region_name} -> {exit_name}", self.player)
                if UNIVERSAL in rulesets:
                    set_rule(entrance, lambda state: False)
                    for rule in rulesets[UNIVERSAL]:
                        add_rule(entrance, rule, "or")
                if difficulty in rulesets:
                    for rule in rulesets[difficulty]:
                        add_rule(entrance, rule, "or")

        for location_name, rulesets in self.location_rules.items():
            location = multiworld.get_location(location_name, self.player)
            if UNIVERSAL in rulesets:
                set_rule(location, lambda state: False)
                for rule in rulesets[UNIVERSAL]:
                    add_rule(location, rule, "or")
            if difficulty in rulesets:
                for rule in rulesets[difficulty]:
                    add_rule(location, rule, "or")

    def has_breaker(self, state) -> bool:
        return state.has("Dream Breaker", self.player)

    def has_slide(self, state) -> bool:
        return state.has("Slide", self.player)

    def has_plunge(self, state) -> bool:
        return state.has("Sunsetter", self.player)

    def has_gem(self, state) -> bool:
        return state.has("Cling Gem", self.player)

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
