from BaseClasses import CollectionState
from typing import Dict, Set, Callable, TYPE_CHECKING
from worlds.generic.Rules import set_rule

if TYPE_CHECKING:
    from . import PseudoregaliaWorld
else:
    PseudoregaliaWorld = object


class PseudoregaliaRules:
    world: PseudoregaliaWorld
    player: int
    region_rules: Dict[str, Callable[[CollectionState], bool]]
    location_rules: Dict[str, Callable[[CollectionState], bool]]

    def __init__(self, world: PseudoregaliaWorld) -> None:
        self.world = world
        self.player = world.player

        self.region_rules = {
            "Dungeon Mirror -> Dungeon Strong Eyes": lambda state:
                self.has_slide(state) and self.has_breaker(state),
            "Dungeon Mirror -> Underbelly Main": lambda state:
                self.has_breaker(state),
            "Dungeon Mirror -> Theatre Main": lambda state:
                self.has_gem(state) and self.get_kicks(state, 3)
                or self.has_gem(state) and self.can_slidejump(state),
            "Dungeon Strong Eyes -> Castle Main": lambda state:
                self.has_small_keys(state),
            "Library Main -> Library Locked": lambda state:
                self.has_small_keys(state),
            "Keep Main -> Keep Sunsetter": lambda state:
                self.has_gem(state)
                or self.has_small_keys(state)
                or self.get_kicks(state, 3),
            "Keep Main -> Underbelly Hole": lambda state:
                self.kick_or_plunge(state, 1),
            "Keep Main -> Theatre Main": lambda state:
                self.has_gem(state) and self.get_kicks(state, 3)
                or self.has_gem(state) and self.can_slidejump(state),
            "Empty Bailey -> Castle Main": lambda state: True,
            "Empty Bailey -> Theatre Pillar": lambda state: True,
            "Empty Bailey -> Tower Remains": lambda state:
                self.has_gem(state)
                or state.has_all({"Slide", "Sunsetter"}, self.player)
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
                state.has_all({"Sunsetter", "Cling Gem"}, self.player)
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
                self.kick_or_plunge(state, 1)
                or self.has_gem(state),
            "Keep - Levers Room": lambda state:
                self.has_breaker(state),
            "Keep - Alcove Near Locked Door": lambda state:
                self.can_slidejump(state)
                or self.get_kicks(state, 3)
                or self.has_plunge(state),
            "Keep - Major Key": lambda state:
                self.can_bounce(state) and state.has_all({"Cling Gem, Sunsetter"}, self.player)
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
                or state.has_all({"Sunsetter", "Slide"}, self.player),
            "Bailey - Major Key": lambda state:
                self.has_plunge(state)
                or self.has_gem(state)
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
                    or self.kick_or_plunge(state, 1)),
            "Underbelly - Main Room": lambda state:
                self.can_slidejump(state)
                or self.has_plunge(state),
            "Underbelly - Alcove Near Light": lambda state: True,
            "Underbelly - Building Near Little Guy": lambda state:
                self.get_kicks(state, 3)
                or self.has_plunge(state),
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
        """Used where either breaker or sunsetter will work."""
        return state.has_any({"Dream Breaker", "Sunsetter"}, self.player)

    def get_kicks(self, state, count: int) -> bool:
        kicks: int = 0
        if (state.has("Sun Greaves", self.player)):
            kicks += 3
        kicks += state.count("Heliacal Power", self.player)
        return kicks >= count

    def kick_or_plunge(self, state, count: int) -> bool:
        """Used where one air kick can be replaced with sunsetter. Input is the number of kicks needed without plunge."""
        total: int = 0
        if (state.has("Sun Greaves", self.player)):
            total += 3
        if (state.has("Sunsetter", self.player)):
            total += 1
        total += state.count("Heliacal Power", self.player)
        return total >= count

    def has_small_keys(self, state) -> bool:
        return (state.count("Small Key", self.player) >= 7)

    def navigate_darkrooms(self, state) -> bool:
        return (state.has("Dream Breaker", self.player) or state.has("Ascendant Light", self.player))

    def can_slidejump(self, state) -> bool:
        return (state.has_all({"Slide", "Solar Wind"}, self.player))

    def can_strikebreak(self, state) -> bool:
        return (state.has_all({"Dream Breaker", "Strikebreak"}, self.player))

    def can_soulcutter(self, state) -> bool:
        return (state.has_all({"Dream Breaker", "Strikebreak", "Soul Cutter"}, self.player))

    def knows_obscure(self, state) -> bool:
        # TODO: rules that use this and functions like it should go in a separate extra_rules dictionary
        return self.world.options.obscure_tricks

    def set_pseudoregalia_rules(self) -> None:
        multiworld = self.world.multiworld

        for region in multiworld.get_regions(self.player):
            for entrance in region.entrances:
                if entrance.name in self.region_rules:
                    set_rule(entrance, self.region_rules[entrance.name])
            for location in region.locations:
                if location.name in self.location_rules:
                    set_rule(location, self.location_rules[location.name])

        set_rule(multiworld.get_location("D S T RT ED M M O   Y", self.player), lambda state:
                 state.has_all({
                     "Major Key - Empty Bailey",
                     "Major Key - The Underbelly",
                     "Major Key - Tower Remains",
                     "Major Key - Sansa Keep",
                     "Major Key - Twilight Theatre",
                 }, self.player))
        multiworld.completion_condition[self.player] = lambda state: state.has(
            "Something Worth Being Awake For", self.player)


class PseudoregaliaNormalRules(PseudoregaliaRules):
    def __init__(self, world) -> None:
        super().__init__(world)

        self.region_rules.update({
            "Castle Main -> Library Main": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.can_attack(state),
            "Castle Main -> Theatre Pillar": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.kick_or_plunge(state, 2),
            "Castle Main -> Castle Spiral Climb": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state) and self.has_plunge(state),
            "Castle Spiral Climb -> Castle High Climb": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3) and self.has_plunge(state)
                or self.has_breaker(state) and self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.has_plunge(state) and self.get_kicks(state, 1),
            "Castle Spiral Climb -> Castle By Scythe Corridor": lambda state:
                self.has_gem(state),
            "Castle By Scythe Corridor -> Castle Spiral Climb": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 4) and self.has_plunge(state),
            "Castle By Scythe Corridor -> Castle => Theatre (Front)": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 2),
            "Castle By Scythe Corridor -> Castle High Climb": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 4)
                or self.get_kicks(state, 2) and self.has_plunge(state)
                or self.get_kicks(state, 1) and self.has_plunge(state) and self.can_slidejump(state),
            "Castle => Theatre (Front) -> Castle By Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.can_slidejump(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 4),
            "Castle => Theatre (Front) -> Castle Moon Room": lambda state:
                self.has_gem(state)
                or self.can_slidejump(state) and self.kick_or_plunge(state, 2),
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
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.get_kicks(state, 2),
            "Castle - Alcove Near Dungeon": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.kick_or_plunge(state, 2),
            "Castle - Balcony": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 3)
                or self.can_slidejump(state) and self.kick_or_plunge(state, 2),
            "Castle - Corner Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 4),
            "Castle - Wheel Crawlers": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2)
                or self.get_kicks(state, 1) and self.can_slidejump(state),
            "Castle - Alcove Near Scythe Corridor": lambda state:
                self.has_gem(state) and self.get_kicks(state, 1) and self.has_plunge(state)
                or self.kick_or_plunge(state, 4),
            "Castle - Near Theatre Front": lambda state:
                self.get_kicks(state, 4)
                or self.get_kicks(state, 2) and self.has_plunge(state),
            "Castle - High Climb From Courtyard": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state) and self.has_plunge(state)
                or self.has_breaker(state) and self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.has_plunge(state) and self.get_kicks(state, 1),
        })

    def set_pseudoregalia_rules(self) -> None:
        super().set_pseudoregalia_rules()


class PseudoregaliaHardRules(PseudoregaliaRules):
    def __init__(self, world) -> None:
        super().__init__(world)

        self.region_rules.update({
            "Castle Main -> Library Main": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.can_attack(state),
            "Castle Main -> Theatre Pillar": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 1),
            "Castle Main -> Castle Spiral Climb": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 2)
                or self.can_slidejump(state) and self.has_plunge(state),
            "Castle Spiral Climb -> Castle High Climb": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 3)
                or self.has_breaker(state) and self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.has_plunge(state) and self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.can_attack(state) and self.can_slidejump(state),
            "Castle Spiral Climb -> Castle By Scythe Corridor": lambda state:
                self.has_gem(state),
            "Castle By Scythe Corridor -> Castle Spiral Climb": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3),
            "Castle By Scythe Corridor -> Castle => Theatre (Front)": lambda state:
                self.has_gem(state),
            "Castle By Scythe Corridor -> Castle High Climb": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 4)
                or self.get_kicks(state, 3) and self.has_breaker(state)
                or self.get_kicks(state, 1) and self.has_plunge(state),
            "Castle => Theatre (Front) -> Castle By Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.can_slidejump(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 4),
            "Castle => Theatre (Front) -> Castle Moon Room": lambda state:
                self.has_gem(state)
                or self.can_slidejump(state) and self.kick_or_plunge(state, 2)
                or self.get_kicks(state, 4),
        })

        self.location_rules.update({
            "Castle - Floater in Courtyard": lambda state:
                self.can_bounce(state) and self.has_plunge(state)
                or self.can_bounce(state) and self.get_kicks(state, 1)
                or self.kick_or_plunge(state, 4)
                or self.has_gem(state),
            "Castle - Locked Door": lambda state:
                self.has_small_keys(state)
                and (
                    self.has_breaker(state)
                    or self.knows_obscure(state) and self.can_attack(state)
                ),
            "Castle - Platform In Main Halls": lambda state:
                self.kick_or_plunge(state, 1)
                or self.has_gem(state),
            "Castle - Tall Room Near Wheel Crawlers": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.can_slidejump(state) and self.has_plunge(state),
            "Castle - Alcove Near Dungeon": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 1),
            "Castle - Balcony": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 3)
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
            "Castle - Alcove Near Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 2) and self.has_plunge(state),
            "Castle - Near Theatre Front": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 4)
                or self.get_kicks(state, 2) and self.has_plunge(state),
            "Castle - High Climb From Courtyard": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state)
                or self.has_plunge(state) and self.can_slidejump(state)
                or self.has_breaker(state) and self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.has_plunge(state) and self.get_kicks(state, 1),
        })

    def set_pseudoregalia_rules(self) -> None:
        super().set_pseudoregalia_rules()


class PseudoregaliaExpertRules(PseudoregaliaRules):
    def __init__(self, world) -> None:
        super().__init__(world)

        self.region_rules.update({
            "Castle Main -> Library Main": lambda state:
                self.can_attack(state),
            "Castle Main -> Theatre Pillar": lambda state:
                self.has_gem(state)
                or self.has_slide(state)
                or self.kick_or_plunge(state, 1),
            "Castle Main -> Castle Spiral Climb": lambda state:
                self.has_gem(state)
                or self.has_slide(state)
                or self.kick_or_plunge(state, 2),
            "Castle Spiral Climb -> Castle High Climb": lambda state: True,
                # Anything that gets you into spiral climb can get from there to high climb
            "Castle Spiral Climb -> Castle By Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 4),
            "Castle By Scythe Corridor -> Castle Spiral Climb": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3),
            "Castle By Scythe Corridor -> Castle => Theatre (Front)": lambda state:
                self.has_gem(state)
                or self.has_slide(state) and self.get_kicks(state, 2),
            "Castle By Scythe Corridor -> Castle High Climb": lambda state:
                self.has_gem(state)
                or self.has_slide(state)
                or self.kick_or_plunge(state, 2),
            "Castle => Theatre (Front) -> Castle By Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.has_slide(state)
                or self.get_kicks(state, 3),
            "Castle => Theatre (Front) -> Castle Moon Room": lambda state:
                self.has_gem(state)
                or self.has_slide(state)
                or self.get_kicks(state, 4),

        })

        self.location_rules.update({
            "Castle - Floater in Courtyard": lambda state:
                self.can_bounce(state)
                and (
                    self.kick_or_plunge(state)
                    or self.has_slide(state))
                or self.has_slide(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 3)
                or self.has_gem(state),
            "Castle - Locked Door": lambda state:
                self.has_small_keys(state) and self.can_attack(state),
            "Castle - Platform In Main Halls": lambda state:
                self.kick_or_plunge(state, 1)
                or self.has_gem(state)
                or self.has_slide(state),
            "Castle - Tall Room Near Wheel Crawlers": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state),
            "Castle - Alcove Near Dungeon": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 1)
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
                or self.kick_or_plunge(state, 1)
                or self.has_slide(state),
            "Castle - Alcove Near Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 3)
                or self.has_slide(state) and self.kick_or_plunge(state, 1),
            "Castle - Near Theatre Front": lambda state:
                self.has_gem(state)
                or self.has_slide(state)
                or self.get_kicks(state, 4)
                or self.get_kicks(state, 2) and self.has_plunge(state),
            "Castle - High Climb From Courtyard": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state)
                or self.can_attack(state) and self.get_kicks(state, 1)
                or self.has_slide(state),
        })

    def set_pseudoregalia_rules(self) -> None:
        super().set_pseudoregalia_rules()


class PseudoregaliaLunaticRules(PseudoregaliaRules):
    def __init__(self, world) -> None:
        super().__init__(world)

        self.region_rules.update({
            "Castle Main -> Library Main": lambda state:
                self.can_attack(state),
            "Castle Main -> Theatre Pillar": lambda state:
                self.has_gem(state)
                or self.has_slide(state)
                or self.kick_or_plunge(state, 1),
            "Castle Main -> Castle Spiral Climb": lambda state:
                self.has_gem(state)
                or self.has_slide(state)
                or self.kick_or_plunge(state, 2),
            "Castle Spiral Climb -> Castle High Climb": lambda state: True,
                # Anything that gets you into spiral climb can get from there to high climb
            "Castle Spiral Climb -> Castle By Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3),
            "Castle By Scythe Corridor -> Castle Spiral Climb": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3),
            "Castle By Scythe Corridor -> Castle => Theatre (Front)": lambda state:
                self.has_gem(state)
                or self.has_slide(state) and self.kick_or_plunge(state, 2),
            "Castle By Scythe Corridor -> Castle High Climb": lambda state:
                self.has_gem(state)
                or self.has_slide(state)
                or self.kick_or_plunge(state, 2),
            "Castle => Theatre (Front) -> Castle By Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.has_slide(state)
                or self.get_kicks(state, 3),
            "Castle => Theatre (Front) -> Castle Moon Room": lambda state:
                self.has_gem(state)
                or self.has_slide(state)
                or self.get_kicks(state, 4),
        })

        self.location_rules.update({
            "Castle - Floater in Courtyard": lambda state:
                self.can_bounce(state)
                and (
                    self.kick_or_plunge(state)
                    or self.has_slide(state))
                or self.has_slide(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 3)
                or self.has_gem(state),
            "Castle - Locked Door": lambda state:
                self.has_small_keys(state) and self.can_attack(state),
            "Castle - Platform In Main Halls": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state)
                or self.has_slide(state)
                or self.can_bounce(state),
            "Castle - Tall Room Near Wheel Crawlers": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state),
            "Castle - Alcove Near Dungeon": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 1)
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
                or self.kick_or_plunge(state, 1)
                or self.has_slide(state),
            "Castle - Alcove Near Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 1),  # This never really matters and that makes me sad
            "Castle - Near Theatre Front": lambda state:
                self.has_gem(state)
                or self.has_slide(state)
                or self.get_kicks(state, 4)
                or self.get_kicks(state, 2) and self.has_plunge(state),
            "Castle - High Climb From Courtyard": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state)
                or self.can_attack(state) and self.get_kicks(state, 1)
                or self.has_slide(state),
        })

    def set_pseudoregalia_rules(self) -> None:
        super().set_pseudoregalia_rules()
