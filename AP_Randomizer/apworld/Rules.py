from BaseClasses import CollectionState
from typing import Dict, Set, Callable
from worlds.generic.Rules import set_rule


class PseudoregaliaRules:
    player: int

    region_rules: Dict[str, Callable[[CollectionState], bool]]
    location_rules: Dict[str, Callable[[CollectionState], bool]]

    def __init__(self, player) -> None:
        self.player = player
        self.region_rules = {
            "Dungeon Mirror -> Dungeon Strong Eyes": lambda state:
                self.has_slide(state) and self.has_breaker(state),
            "Dungeon Mirror -> Underbelly Main": lambda state:
                self.has_breaker(state),
            "Dungeon Mirror -> Theatre Main": lambda state:
                self.has_gem(state) and self.get_kicks(state, 3)
                or self.has_gem(state) and self.can_slidejump(state),
            "Dungeon Strong Eyes -> Castle Sansa": lambda state:
                self.has_small_keys(state),
            "Castle Sansa -> Library Main": lambda state: False,
            "Castle Sansa -> Keep Main": lambda state: True,
            "Castle Sansa -> Empty Bailey": lambda state: True,
            "Castle Sansa -> Theatre Pillar": lambda state: False,
            "Castle Sansa -> Theatre Main": lambda state:
                self.has_gem(state)
                or self.can_slidejump(state) and self.get_kicks(state, 4),
            "Library Main -> Library Locked": lambda state:
                self.has_small_keys(state),
            "Keep Main -> Keep Sunsetter": lambda state:
                self.has_gem(state)
                or self.has_small_keys(state)
                or self.get_kicks(state, 3),
            "Keep Main -> Underbelly Hole": lambda state:
                self.has_plunge(state)
                or self.get_kicks(state, 1),
            "Keep Main -> Theatre Main": lambda state:
                self.has_gem(state) and self.get_kicks(state, 3)
                or self.has_gem(state) and self.can_slidejump(state),
            "Empty Bailey -> Castle Sansa": lambda state: True,
            "Empty Bailey -> Theatre Pillar": lambda state: True,
            "Empty Bailey -> Tower Remains": lambda state:
                self.has_gem(state)
                or state.has_all(["Slide", "Sunsetter"], self.player)
                or self.get_kicks(state, 1),
            "Tower Remains -> Underbelly Main": lambda state:
                self.has_plunge(state),
            "Tower Remains -> The Great Door": lambda state:
                self.has_gem(state) and self.get_kicks(state, 3),
            "Underbelly Main -> Empty Bailey": lambda state:
                self.has_plunge(state)
                or self.has_breaker(state),
            "Underbelly Hole -> Underbelly Main": lambda state:
                self.has_plunge(state),
            "Theatre Main -> Keep Main": lambda state:
                self.has_gem(state),
            "Theatre Pillar -> Theatre Main": lambda state:
                state.has_all(["Sunsetter", "Cling Gem"], self.player)
                or self.has_plunge(state) and self.get_kicks(state, 4),
        }

        self.location_rules = {
            # TODO: figure out a proper difference between placeholder "free" rules
            # and rules with no universal access(?)
            "Dungeon - Dream Breaker": lambda state: True,
            "Dungeon - Slide": lambda state:
                self.has_breaker(state),
            "Dungeon - Dark Orbs": lambda state:
                self.has_gem(state)
                or self.can_bounce(state) and self.get_kicks(state, 3),
            "Dungeon - Rafters": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3) and self.can_slidejump(state)
                or self.get_kicks(state, 3) and self.has_plunge(state),
            "Dungeon - Strong Eyes": lambda state:
                self.has_breaker(state),
            "Dungeon - Alcove Near Mirror": lambda state: True,
            "Dungeon - Past Poles": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3),
            "Castle - Indignation": lambda state: True,
            "Castle - Floater In Courtyard": lambda state:
                self.can_bounce(state) and self.has_plunge(state),
            "Castle - High Climb From Courtyard": lambda state:
                self.has_gem(state)
                or self.can_slidejump(state) and self.get_kicks(state, 1),
            "Castle - Locked Door": lambda state: False,
            "Castle - Near Theatre Front": lambda state:
                self.has_gem(state) and self.get_kicks(state, 3),
            "Castle - Platform In Main Halls": lambda state:
                self.has_plunge(state) and self.has_gem(state),
            "Castle - Tall Room Near Wheel Crawlers": lambda state: False,
            "Castle - Alcove Near Dungeon": lambda state: False,
            "Castle - Alcove Near Scythe Corridor": lambda state:
                self.can_slidejump(state) and self.get_kicks(state, 4)
                or self.has_gem(state),
            "Castle - Balcony": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3),
            "Castle - Corner Corridor": lambda state:
                self.has_gem(state),
            "Castle - Wheel Crawlers": lambda state:
                self.can_bounce(state)
                or self.has_gem(state),
            "Library - Sun Greaves": lambda state:
                self.can_slidejump(state)
                or self.has_slide(state) and self.has_breaker(state)
                or self.has_gem(state) and self.has_breaker(state)
                or self.get_kicks(state, 4),
            "Library - Locked Door Left": lambda state:
                self.get_kicks(state, 3)
                or self.has_gem(state),
            "Library - Locked Door Across": lambda state:
                self.can_slidejump(state)
                or self.get_kicks(state, 1)
                or self.has_gem(state),
            "Library - Upper Back": lambda state:
                self.get_kicks(state, 3)
                or self.has_gem(state),
            "Keep - Strikebreak": lambda state:
                (self.has_slide(state) or self.can_strikebreak(state))
                and (
                    self.can_slidejump(state)
                    or self.get_kicks(state, 1)
                    or self.has_gem(state),
                ),
            "Keep - Sunsetter": lambda state:
                self.has_breaker(state),
            "Keep - Near Theatre": lambda state:
                self.has_plunge(state)
                or self.has_gem(state)
                or self.get_kicks(state, 1),
            "Keep - Levers Room": lambda state:
                self.has_breaker(state),
            "Keep - Alcove Near Locked Door": lambda state:
                self.can_slidejump(state)
                or self.get_kicks(state, 3)
                or self.has_plunge(state),
            "Keep - Major Key": lambda state:
                self.can_bounce(state) and state.has_all(["Cling Gem, Sunsetter"], self.player)
                or self.has_gem(state) and self.can_bounce(state) and self.get_kicks(state, 3),
            "Bailey - Solar Wind": lambda state:
                self.has_slide(state),
            "Bailey - Cheese Bell": lambda state:
                self.can_slidejump(state) and self.get_kicks(state, 1) and self.has_plunge(state)
                or self.can_slidejump(state) and self.has_gem(state)
                or self.get_kicks(state, 3) and self.has_plunge(state),
            "Bailey - Inside Building": lambda state:
                self.has_slide(state),
            "Bailey - Center Steeple": lambda state:
                self.get_kicks(state, 3)
                or state.has_all(["Sunsetter", "Slide"], self.player),
            "Bailey - Major Key": lambda state:
                self.has_plunge
                or self.has_gem
                or self.get_kicks(state, 3),
            "Theatre - Soul Cutter": lambda state:
                self.can_strikebreak(state),
            "Theatre - Corner Beam": lambda state:
                self.has_gem(state) and self.get_kicks(state, 3)
                or self.has_gem(state) and self.can_slidejump(state)
                or self.get_kicks(state, 3) and self.can_slidejump(state),
            "Theatre - Locked Door": lambda state:
                self.has_small_keys(state)
                or self.get_kicks(state, 3)
                or self.has_gem(state),
            "Theatre - Back Of Auditorium": lambda state:
                self.get_kicks(state, 3)
                or self.has_gem(state),
            "Theatre - Murderous Goat": lambda state: True,
            "Theatre - Major Key": lambda state:
                self.can_soulcutter(state) and self.has_gem(state) and self.can_slidejump(state)
                or self.can_soulcutter(state) and self.has_gem(state) and self.get_kicks(state, 1),
            "Underbelly - Ascendant Light": lambda state: True,
            "Underbelly - Locked Door": lambda state:
                self.has_small_keys(state) and self.has_slide(state) and self.get_kicks(state, 3)
                or self.has_small_keys(state) and self.has_slide(state) and self.has_plunge(state),
            "Underbelly - Strikebreak Wall": lambda state:
                (self.can_strikebreak(state) and self.can_bounce(state))
                and (
                    self.can_slidejump(state)
                    or self.get_kicks(state, 1)
                    or self.has_plunge(state)
                ),
            "Underbelly - Main Room": lambda state:
                self.can_slidejump(state)
                or self.has_plunge(state),
            "Underbelly - Alcove Near Light": lambda state: True,
            "Underbelly - Building Near Little Guy": lambda state:
                self.get_kicks(state, 3)
                or self.has_plunge,
            "Underbelly - Rafters Near Keep": lambda state:
                self.get_kicks(state, 3)
                or self.get_kicks(state, 1) and self.has_plunge(state),
            "Underbelly - Major Key": lambda state:
                self.has_plunge(state)
                and (
                    self.can_soulcutter(state) and self.can_bounce(state)
                    or self.can_soulcutter(state) and self.has_gem(state)
                    or self.can_slidejump(state) and self.get_kicks(state, 3)
                ),
            "Tower - Cling Gem": lambda state:
                self.get_kicks(state, 3),
            "Tower - Major Key": lambda state: True,
        }

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

    def can_attack(self, state) -> bool:
        return state.has_any(["Dream Breaker", "Sunsetter"], self.player)

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

    def knows_obscure(self, state) -> bool:
        # Will check the obscure setting and return that
        return False

    def set_pseudoregalia_rules(self, multiworld) -> None:
        for region in multiworld.get_regions(self.player):
            for entrance in region.entrances:
                if entrance.name in self.region_rules:
                    set_rule(entrance, self.region_rules[entrance.name])
            for location in region.locations:
                if location.name in self.location_rules:
                    set_rule(location, self.location_rules[location.name])


class PseudoregaliaNormalRules(PseudoregaliaRules):
    def __init__(self, player) -> None:
        super().__init__(player)
        self.player = player

        self.region_rules.update({
            "Castle Sansa -> Library Main": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.can_attack(state),
            "Castle Sansa -> Theatre Pillar": lambda state:
                self.has_gem(state) and self.has_plunge(state)
                or self.has_gem(state) and self.get_kicks(state, 1)
                or self.has_plunge(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 2),
        })

        self.location_rules.update({
            "Castle - Floater in Courtyard": lambda state:
                self.can_bounce(state) and self.has_plunge(state)
                or self.can_bounce(state) and self.get_kicks(state, 2)
                or self.has_gem(state) and self.get_kicks(state, 2)
                or self.has_gem(state) and self.has_plunge(state)
                or self.get_kicks(state, 4),
            "Castle - Locked Door": lambda state:
                self.has_small_keys(state)
                and (
                    self.has_breaker(state)
                    or self.knows_obscure(state) and self.can_attack(state)
                ),
            "Castle - Platform In Main Halls": lambda state:
                self.has_plunge(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2),
            "Castle - Tall Room Near Wheel Crawlers": lambda state:
                self.has_gem(state) and self.has_plunge(state)
                or self.has_gem(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 2),
            "Castle - Alcove Near Dungeon": lambda state:
                self.has_gem(state) and self.has_plunge(state)
                or self.has_gem(state) and self.get_kicks(state, 1)
                or self.has_plunge(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 2),
            "Castle - Balcony": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3)
                or self.has_plunge(state) and self.get_kicks(state, 2)
                or self.can_slidejump(state) and self.get_kicks(state, 2)
                or self.can_slidejump(state) and self.get_kicks(state, 1) and self.has_plunge(state, 1),
            "Castle - Corner Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 4),
            "Castle - Wheel Crawlers": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2)
                or self.get_kicks(state, 1) and self.can_slidejump(state),

        })

    def set_pseudoregalia_rules(self, multiworld) -> None:
        super().set_pseudoregalia_rules(multiworld)


class PseudoregaliaHardRules(PseudoregaliaRules):
    def __init__(self, player) -> None:
        super().__init__(player)
        self.player = player

        self.region_rules.update({
            "Castle Sansa -> Library Main": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.can_attack(state),
            "Castle Sansa -> Theatre Pillar": lambda state:
                self.has_gem(state)
                or self.has_plunge(state)
                or self.get_kicks(state, 1),
        })

        self.location_rules.update({
            "Castle - Floater in Courtyard": lambda state:
                self.can_bounce(state) and self.has_plunge(state)
                or self.can_bounce(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 4)
                or self.get_kicks(state, 3) and self.has_plunge(state)
                or self.has_gem(state),
            "Castle - Locked Door": lambda state:
                self.has_small_keys(state)
                and (
                    self.has_breaker(state)
                    or self.knows_obscure(state) and self.can_attack(state)
                ),
            "Castle - Platform In Main Halls": lambda state:
                self.has_plunge(state)
                or self.has_gem(state)
                or self.get_kicks(state, 1),
            "Castle - Tall Room Near Wheel Crawlers": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.can_slidejump(state) and self.has_plunge(state),
            "Castle - Alcove Near Dungeon": lambda state:
                self.has_gem(state)
                or self.has_plunge(state)
                or self.get_kicks(state, 1),
            "Castle - Balcony": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3)
                or self.has_plunge(state) and self.get_kicks(state, 2)
                or self.has_slide(state) and self.has_plunge(state)
                or self.has_slide(state) and self.get_kicks(state, 1) and self.has_breaker(state),
            "Castle - Corner Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3),
            "Castle - Wheel Crawlers": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.can_slidejump(state) and self.has_plunge(state)
                or self.knows_obscure(state) and self.has_plunge(state),
        })

    def set_pseudoregalia_rules(self, multiworld) -> None:
        super().set_pseudoregalia_rules(multiworld)


class PseudoregaliaExpertRules(PseudoregaliaRules):
    def __init__(self, player) -> None:
        super().__init__(player)
        self.player = player

        self.region_rules.update({
            "Castle Sansa -> Library Main": lambda state:
                self.can_attack(state),
            "Castle Sansa -> Theatre Pillar": lambda state:
                self.has_gem(state)
                or self.has_plunge(state)
                or self.has_slide(state)
                or self.get_kicks(state, 1),
        })

        self.location_rules.update({
            "Castle - Floater in Courtyard": lambda state:
                self.can_bounce(state)
                and (
                    self.has_plunge(state)
                    or self.get_kicks(state, 1)
                    or self.has_slide(state))
                or self.has_slide(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 3)
                or self.has_gem(state),
            "Castle - Locked Door": lambda state:
                self.has_small_keys(state) and self.can_attack(state),
            "Castle - Platform In Main Halls": lambda state:
                self.has_plunge(state)
                or self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state),
            "Castle - Tall Room Near Wheel Crawlers": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state),
            "Castle - Alcove Near Dungeon": lambda state:
                self.has_gem(state)
                or self.has_plunge(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state),
            "Castle - Balcony": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3)
                or self.has_plunge(state) and self.get_kicks(state, 1)
                or self.has_slide(state),
            "Castle - Corner Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3)
                or self.get_kicks(state, 2) and self.has_slide(state),
            "Castle - Wheel Crawlers": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state)
                or self.has_plunge(state),
        })

    def set_pseudoregalia_rules(self, multiworld) -> None:
        super().set_pseudoregalia_rules(multiworld)


class PseudoregaliaLunaticRules(PseudoregaliaRules):
    def __init__(self, player) -> None:
        super().__init__(player)
        self.player = player

        self.region_rules.update({
            "Castle Sansa -> Library Main": lambda state:
                self.can_attack(state),
            "Castle Sansa -> Theatre Pillar": lambda state:
                self.has_gem(state)
                or self.has_plunge(state)
                or self.has_slide(state)
                or self.get_kicks(state, 1),
        })

        self.location_rules.update({
            "Castle - Floater in Courtyard": lambda state:
                self.can_bounce(state)
                and (
                    self.has_plunge(state)
                    or self.get_kicks(state, 1)
                    or self.has_slide(state))
                or self.has_slide(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 3)
                or self.has_gem(state),
            "Castle - Locked Door": lambda state:
                self.has_small_keys(state) and self.can_attack(state),
            "Castle - Platform In Main Halls": lambda state:
                self.has_plunge(state)
                or self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state)
                or self.can_bounce(state),
            "Castle - Tall Room Near Wheel Crawlers": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state),
            "Castle - Alcove Near Dungeon": lambda state:
                self.has_gem(state)
                or self.has_plunge(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state),
            "Castle - Balcony": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3)
                or self.has_plunge(state) and self.get_kicks(state, 1)
                or self.has_slide(state),
            "Castle - Corner Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3)
                or self.get_kicks(state, 1) and self.has_slide(state),
            "Castle - Wheel Crawlers": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state)
                or self.has_plunge(state),
        })

    def set_pseudoregalia_rules(self, multiworld) -> None:
        super().set_pseudoregalia_rules(multiworld)
