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
            "Theatre Outside Scythe Corridor -> Theatre Main": lambda state:
                self.has_gem(state) and self.get_kicks(state, 3)
                or self.has_gem(state) and self.can_slidejump(state),
        }

        self.location_rules = {
            # TODO: figure out a proper difference between placeholder "free" rules
            # and rules with no universal access(?)
            "Sansa Keep - Strikebreak": lambda state:
                self.can_slidejump(state)
                or self.has_slide(state) and self.get_kicks(state, 1)
                or self.has_slide(state) and self.has_gem(state)
                or self.can_strikebreak(state) and self.get_kicks(state, 1)
                or self.can_strikebreak(state) and self.has_gem(state),
            "Sansa Keep - Sunsetter": lambda state:
                self.has_breaker(state),
            "Sansa Keep - Near Theatre": lambda state:
                self.kick_or_plunge(state, 1)
                or self.has_gem(state),
            "Sansa Keep - Levers Room": lambda state:
                self.has_breaker(state),
            "Sansa Keep - Alcove Near Locked Door": lambda state:
                self.can_slidejump(state)
                or self.get_kicks(state, 3)
                or self.has_plunge(state),
            "Sansa Keep - Lonely Throne": lambda state:
                self.can_bounce(state) and state.has_all({"Cling Gem, Sunsetter"}, self.player)
                or self.has_gem(state) and self.can_bounce(state) and self.get_kicks(state, 3),
            "Empty Bailey - Solar Wind": lambda state:
                self.has_slide(state),
            "Empty Bailey - Cheese Bell": lambda state:
                self.can_slidejump(state) and self.get_kicks(state, 1) and self.has_plunge(state)
                or self.can_slidejump(state) and self.has_gem(state)
                or self.get_kicks(state, 3) and self.has_plunge(state),
            "Empty Bailey - Inside Building": lambda state:
                self.has_slide(state),
            "Empty Bailey - Center Steeple": lambda state:
                self.get_kicks(state, 3)
                or state.has_all({"Sunsetter", "Slide"}, self.player),
            "Empty Bailey - Guarded Hand": lambda state:
                self.has_plunge(state)
                or self.has_gem(state)
                or self.get_kicks(state, 3),
            "Twilight Theatre - Soul Cutter": lambda state:
                self.can_strikebreak(state),
            "Twilight Theatre - Corner Beam": lambda state:
                self.has_gem(state) and self.get_kicks(state, 3)
                or self.has_gem(state) and self.can_slidejump(state)
                or self.get_kicks(state, 3) and self.can_slidejump(state),
            "Twilight Theatre - Locked Door": lambda state:
                self.has_small_keys(state)
                and (
                    self.has_gem(state)
                    or self.get_kicks(state, 3)),
            "Twilight Theatre - Back Of Auditorium": lambda state:
                self.get_kicks(state, 3)
                or self.has_gem(state),
            "Twilight Theatre - Murderous Goat": lambda state: True,
            "Twilight Theatre - Center Stage": lambda state:
                self.can_soulcutter(state) and self.has_gem(state) and self.can_slidejump(state)
                or self.can_soulcutter(state) and self.has_gem(state) and self.get_kicks(state, 1),
            "The Underbelly - Ascendant Light": lambda state: True,
            "The Underbelly - Locked Door": lambda state:
                (self.has_small_keys(state) and self.has_slide(state))
                and (
                    self.get_kicks(state, 3)
                    or self.has_plunge(state)),
            "The Underbelly - Strikebreak Wall": lambda state:
                (self.can_strikebreak(state) and self.can_bounce(state))
                and (
                    self.can_slidejump(state)
                    or self.kick_or_plunge(state, 1)),
            "The Underbelly - Main Room": lambda state:
                self.can_slidejump(state)
                or self.has_plunge(state),
            "The Underbelly - Alcove Near Light": lambda state: True,
            "The Underbelly - Building Near Little Guy": lambda state:
                self.get_kicks(state, 3)
                or self.has_plunge(state),
            "The Underbelly - Rafters Near Keep": lambda state:
                self.get_kicks(state, 3)
                or self.get_kicks(state, 1) and self.has_plunge(state),
            "The Underbelly - Surrounded By Holes": lambda state:
                self.has_plunge(state)
                and (
                    self.can_soulcutter(state) and self.can_bounce(state)
                    or self.can_soulcutter(state) and self.has_gem(state)
                    or self.can_slidejump(state) and self.get_kicks(state, 3)
                ),
            "Tower Remains - Cling Gem": lambda state:
                self.get_kicks(state, 3),
            "Tower Remains - Atop The Tower": lambda state: True,
        }

    def has_breaker(self, state) -> bool:
        return state.has_any({"Dream Breaker", "Progressive Dream Breaker"}, self.player)

    def has_slide(self, state) -> bool:
        return state.has_any({"Slide", "Progressive Slide"}, self.player)

    def has_plunge(self, state) -> bool:
        return state.has("Sunsetter", self.player)

    def has_gem(self, state) -> bool:
        return state.has("Cling Gem", self.player)

    def can_bounce(self, state) -> bool:
        return self.has_breaker(state) and state.has("Ascendant Light", self.player)

    def can_attack(self, state) -> bool:
        """Used where either breaker or sunsetter will work."""
        # TODO: Update this to check obscure tricks when logic rework nears completion
        return self.has_breaker(state) or state.has("Sunsetter", self.player)

    def get_kicks(self, state, count: int) -> bool:
        kicks: int = 0
        if (state.has("Sun Greaves", self.player)):
            kicks += 3
        kicks += state.count("Heliacal Power", self.player)
        kicks += state.count("Air Kick", self.player)
        return kicks >= count

    def kick_or_plunge(self, state, count: int) -> bool:
        """Used where one air kick can be replaced with sunsetter. Input is the number of kicks needed without plunge."""
        total: int = 0
        if (state.has("Sun Greaves", self.player)):
            total += 3
        if (state.has("Sunsetter", self.player)):
            total += 1
        total += state.count("Heliacal Power", self.player)
        total += state.count("Air Kick", self.player)
        return total >= count

    def has_small_keys(self, state) -> bool:
        # TODO: This needs to check for can_attack once breaker can be shuffled
        return (state.count("Small Key", self.player) >= 7)

    def navigate_darkrooms(self, state) -> bool:
        # TODO: Update this to check obscure tricks for breaker only when logic rework nears completion
        return self.has_breaker(state) or state.has("Ascendant Light", self.player)

    def can_slidejump(self, state) -> bool:
        return (state.has_all({"Slide", "Solar Wind"}, self.player)
                or state.count("Progressive Slide", self.player) >= 2)

    def can_strikebreak(self, state) -> bool:
        return (state.has_all({"Dream Breaker", "Strikebreak"}, self.player)
                or state.count("Progressive Dream Breaker", self.player) >= 2)

    def can_soulcutter(self, state) -> bool:
        return (state.has_all({"Dream Breaker", "Strikebreak", "Soul Cutter"}, self.player)
                or state.count("Progressive Dream Breaker", self.player) >= 3)

    def knows_obscure(self, state) -> bool:
        return bool(self.world.multiworld.obscure_logic[self.player])

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
            "Dungeon Mirror -> Dungeon Slide": lambda state:
                self.can_attack(state),
            "Dungeon Slide -> Dungeon Mirror": lambda state:
                self.can_attack(state),
            "Dungeon Slide -> Dungeon Strong Eyes": lambda state:
                self.has_slide(state),
            "Dungeon Slide -> Dungeon Escape Lower": lambda state:
                self.can_attack(state) and self.navigate_darkrooms(state),
            "Dungeon Strong Eyes -> Dungeon Slide": lambda state:
                self.has_slide(state),
            "Dungeon Strong Eyes -> Dungeon => Castle": lambda state:
                self.has_small_keys(state),
            "Dungeon => Castle -> Dungeon Strong Eyes": lambda state:
                self.has_small_keys(state),
            "Dungeon Escape Lower -> Dungeon Slide": lambda state:
                self.can_attack(state),
            "Dungeon Escape Lower -> Dungeon Escape Upper": lambda state:
                self.can_bounce(state)
                or self.get_kicks(state, 1) and self.has_plunge(state)
                or self.get_kicks(state, 3),
            "Dungeon Escape Upper -> Theatre Outside Scythe Corridor": lambda state:
                self.can_bounce(state)
                or self.kick_or_plunge(state, 1)
                or self.has_gem(state),
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
            "Library Main -> Library Locked": lambda state:
                self.has_small_keys(state),
            "Library Main -> Library Greaves": lambda state:
                self.has_slide(state),
            "Library Main -> Library Top": lambda state:
                self.kick_or_plunge(state, 4)
                or self.knows_obscure(state) and self.get_kicks(state, 1) and self.has_plunge(state),
            "Library Greaves -> Library Top": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 2),
            "Library Top -> Library Greaves": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.get_kicks(state, 3) and self.has_plunge(state)
                or self.get_kicks(state, 3) and self.can_bounce(state),
            "Keep Main -> Keep Locked Room": lambda state:
                # Note for trackers: This is accessible with nothing but not in logic.
                # Cutting the platform or hitting the lever make this harder and are irreversible.
                # On Hard and above, the player is expected to not do either.
                self.has_small_keys(state)
                or self.get_kicks(state, 3)
                or self.has_plunge(state) and self.get_kicks(state, 1)
                or self.has_gem(state) and self.has_plunge(state)
                or self.has_gem(state) and self.get_kicks(state, 1),
            "Keep Main -> Keep Sunsetter": lambda state:
                # See "Keep Main -> Keep Locked Room".
                self.has_gem(state),
                # All other methods would go through Keep Locked Room instead
            "Keep Main -> Keep => Underbelly": lambda state:
                self.kick_or_plunge(state, 1)
                or self.has_gem(state),
            "Keep Locked Room -> Keep Sunsetter": lambda state: True,
            "Keep => Underbelly -> Underbelly Hole": lambda state: True,
        })

        self.location_rules.update({
            "Dilapidated Dungeon - Dark Orbs": lambda state:
                self.has_gem(state) and self.can_bounce(state)
                or self.has_gem(state) and self.kick_or_plunge(state, 3)
                or self.get_kicks(state, 2) and self.can_bounce(state)
                or self.can_slidejump(state) and self.get_kicks(state, 1) and self.can_bounce(state),
            "Dilapidated Dungeon - Past Poles": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.get_kicks(state, 3),
            "Dilapidated Dungeon - Rafters": lambda state:
                self.kick_or_plunge(state, 3)
                or self.knows_obscure(state) and self.can_bounce(state) and self.has_gem(state),
            "Dilapidated Dungeon - Strong Eyes": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state)
                and (
                    self.has_gem(state) and self.get_kicks(state, 1) and self.has_plunge(state)
                    or self.has_gem(state) and self.get_kicks(state, 3)),
            "Castle Sansa - Floater In Courtyard": lambda state:
                self.can_bounce(state) and self.has_plunge(state)
                or self.can_bounce(state) and self.get_kicks(state, 2)
                or self.has_gem(state) and self.get_kicks(state, 2)
                or self.has_gem(state) and self.has_plunge(state)
                or self.get_kicks(state, 4)
                or self.knows_obscure(state) and self.can_bounce(state) and self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.has_gem(state) and self.get_kicks(state, 1),
            "Castle Sansa - Locked Door": lambda state:
                self.has_small_keys(state),
            "Castle Sansa - Platform In Main Halls": lambda state:
                self.has_plunge(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2),
            "Castle Sansa - Tall Room Near Wheel Crawlers": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.get_kicks(state, 2),
            "Castle Sansa - Alcove Near Dungeon": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.kick_or_plunge(state, 2),
            "Castle Sansa - Balcony": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 3)
                or self.can_slidejump(state) and self.kick_or_plunge(state, 2),
            "Castle Sansa - Corner Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 4),
            "Castle Sansa - Wheel Crawlers": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2)
                or self.get_kicks(state, 1) and self.can_slidejump(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Castle Sansa - Alcove Near Scythe Corridor": lambda state:
                self.has_gem(state) and self.get_kicks(state, 1) and self.has_plunge(state)
                or self.kick_or_plunge(state, 4),
            "Castle Sansa - Near Theatre Front": lambda state:
                self.get_kicks(state, 4)
                or self.get_kicks(state, 2) and self.has_plunge(state),
            "Castle Sansa - High Climb From Courtyard": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state) and self.has_plunge(state)
                or self.has_breaker(state) and self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.has_plunge(state) and self.get_kicks(state, 1),
            "Listless Library - Sun Greaves": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Listless Library - Sun Greaves 1": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Listless Library - Sun Greaves 2": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Listless Library - Sun Greaves 3": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Listless Library - Upper Back": lambda state:
                (self.has_breaker(state) or self.knows_obscure(state) and self.has_plunge(state))
                and (
                    self.has_gem(state) and self.kick_or_plunge(state, 1)
                    or self.kick_or_plunge(state, 2)),
            "Listless Library - Locked Door Across": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.can_slidejump(state),
            "Listless Library - Locked Door Left": lambda state:
                self.has_gem(state)
                or self.can_slidejump(state) and self.get_kicks(state, 1)
                or self.kick_or_plunge(state, 3)
        })

    def set_pseudoregalia_rules(self) -> None:
        super().set_pseudoregalia_rules()


class PseudoregaliaHardRules(PseudoregaliaRules):
    def __init__(self, world) -> None:
        super().__init__(world)

        self.region_rules.update({
            "Dungeon Mirror -> Dungeon Slide": lambda state:
                self.can_attack(state),
            "Dungeon Slide -> Dungeon Mirror": lambda state:
                self.can_attack(state),
            "Dungeon Slide -> Dungeon Strong Eyes": lambda state:
                self.has_slide(state),
            "Dungeon Slide -> Dungeon Escape Lower": lambda state:
                self.can_attack(state) and self.navigate_darkrooms(state),
            "Dungeon Strong Eyes -> Dungeon Slide": lambda state:
                self.has_slide(state),
            "Dungeon Strong Eyes -> Dungeon => Castle": lambda state:
                self.has_small_keys(state),
            "Dungeon => Castle -> Dungeon Strong Eyes": lambda state:
                self.has_small_keys(state),
            "Dungeon Escape Lower -> Dungeon Slide": lambda state:
                self.can_attack(state),
            "Dungeon Escape Lower -> Dungeon Escape Upper": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.kick_or_plunge(state, 2),
            "Dungeon Escape Upper -> Theatre Outside Scythe Corridor": lambda state:
                self.can_bounce(state)
                or self.kick_or_plunge(state, 1)
                or self.has_gem(state),
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
            "Library Main -> Library Locked": lambda state:
                self.has_small_keys(state),
            "Library Main -> Library Greaves": lambda state:
                self.has_slide(state),
            "Library Main -> Library Top": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 4)
                or self.knows_obscure(state) and self.kick_or_plunge(state, 2),
            "Library Greaves -> Library Top": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1),
            "Library Top -> Library Greaves": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3)
                or self.get_kicks(state, 2) and self.has_plunge(state) and self.can_bounce(state),
        })

        self.location_rules.update({
            "Dilapidated Dungeon - Dark Orbs": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1) and self.can_bounce(state)
                or self.can_slidejump(state) and self.has_plunge(state) and self.can_bounce(state)
                or self.get_kicks(state, 3) and self.has_plunge(state),
            "Dilapidated Dungeon - Past Poles": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 2),
            "Dilapidated Dungeon - Rafters": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3)
                or self.get_kicks(state, 1) and self.has_plunge(state)
                or self.get_kicks(state, 1) and self.can_bounce(state),
            "Dilapidated Dungeon - Strong Eyes": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.has_gem(state) and self.kick_or_plunge(state, 2),
            "Castle Sansa - Floater In Courtyard": lambda state:
                self.can_bounce(state) and self.has_plunge(state)
                or self.can_bounce(state) and self.get_kicks(state, 2)
                or self.kick_or_plunge(state, 4)
                or self.has_gem(state)
                or self.knows_obscure(state) and self.can_bounce(state) and self.get_kicks(state, 1),
            "Castle Sansa - Locked Door": lambda state:
                self.has_small_keys(state),
            "Castle Sansa - Platform In Main Halls": lambda state:
                self.kick_or_plunge(state, 1)
                or self.has_gem(state),
            "Castle Sansa - Tall Room Near Wheel Crawlers": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.can_slidejump(state) and self.has_plunge(state),
            "Castle Sansa - Alcove Near Dungeon": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Castle Sansa - Balcony": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 3)
                or self.has_slide(state) and self.has_plunge(state)
                or self.has_slide(state) and self.get_kicks(state, 1) and self.has_breaker(state),
            "Castle Sansa - Corner Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3),
            "Castle Sansa - Wheel Crawlers": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.can_slidejump(state) and self.has_plunge(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Castle Sansa - Alcove Near Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 2) and self.has_plunge(state),
            "Castle Sansa - Near Theatre Front": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 4)
                or self.get_kicks(state, 2) and self.has_plunge(state),
            "Castle Sansa - High Climb From Courtyard": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state)
                or self.has_plunge(state) and self.can_slidejump(state)
                or self.has_breaker(state) and self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.has_plunge(state) and self.get_kicks(state, 1),
            "Listless Library - Sun Greaves": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Listless Library - Sun Greaves 1": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Listless Library - Sun Greaves 2": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Listless Library - Sun Greaves 3": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Listless Library - Upper Back": lambda state:
                (self.has_breaker(state) or self.knows_obscure(state) and self.has_plunge(state))
                and (
                    self.has_gem(state)
                    or self.kick_or_plunge(state, 2)),
            "Listless Library - Locked Door Across": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 1)
                or self.can_slidejump(state),
            "Listless Library - Locked Door Left": lambda state:
                self.has_gem(state)
                or self.can_slidejump(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 2),
        })

    def set_pseudoregalia_rules(self) -> None:
        super().set_pseudoregalia_rules()


class PseudoregaliaExpertRules(PseudoregaliaRules):
    def __init__(self, world) -> None:
        super().__init__(world)

        self.region_rules.update({
            "Dungeon Mirror -> Dungeon Slide": lambda state:
                self.can_attack(state),
            "Dungeon Slide -> Dungeon Mirror": lambda state:
                self.can_attack(state),
            "Dungeon Slide -> Dungeon Strong Eyes": lambda state:
                self.has_slide(state),
            "Dungeon Slide -> Dungeon Escape Lower": lambda state:
                self.can_attack(state) and self.navigate_darkrooms(state),
            "Dungeon Strong Eyes -> Dungeon Slide": lambda state:
                self.has_slide(state),
            "Dungeon Strong Eyes -> Dungeon => Castle": lambda state:
                self.has_small_keys(state),
            "Dungeon => Castle -> Dungeon Strong Eyes": lambda state:
                self.has_small_keys(state),
            "Dungeon Escape Lower -> Dungeon Slide": lambda state:
                self.can_attack(state),
            "Dungeon Escape Lower -> Dungeon Escape Upper": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.kick_or_plunge(state, 2)
                or self.has_slide(state) and self.get_kicks(state, 1),
            "Dungeon Escape Upper -> Theatre Outside Scythe Corridor": lambda state:
                self.can_bounce(state)
                or self.kick_or_plunge(state, 1)
                or self.has_gem(state)
                or self.has_slide(state),
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
            "Library Main -> Library Locked": lambda state:
                self.has_small_keys(state),
            "Library Main -> Library Greaves": lambda state:
                self.has_slide(state),
            "Library Main -> Library Top": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 2)
                or self.has_slide(state),
            "Library Greaves -> Library Top": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state),
            "Library Top -> Library Greaves": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 2),
        })

        self.location_rules.update({
            "Dilapidated Dungeon - Dark Orbs": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1) and self.can_bounce(state)
                or self.get_kicks(state, 3) and self.has_plunge(state)
                or self.has_slide(state) and self.get_kicks(state, 1)
                or self.has_slide(state) and self.can_bounce(state),
            "Dilapidated Dungeon - Past Poles": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 2),
            "Dilapidated Dungeon - Rafters": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 2)
                or self.can_bounce(state) and self.get_kicks(state, 1)
                or self.has_slide(state) and self.kick_or_plunge(state, 1),
            "Dilapidated Dungeon - Strong Eyes": lambda state:
                self.has_breaker(state)
                or self.has_gem(state)
                or self.has_slide(state) and self.get_kicks(state, 1),
            "Castle Sansa - Floater In Courtyard": lambda state:
                self.can_bounce(state)
                and (
                    self.kick_or_plunge(state, 1)
                    or self.has_slide(state))
                or self.has_slide(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 3)
                or self.has_gem(state),
            "Castle Sansa - Locked Door": lambda state:
                self.has_small_keys(state),
            "Castle Sansa - Platform In Main Halls": lambda state:
                self.kick_or_plunge(state, 1)
                or self.has_gem(state)
                or self.has_slide(state),
            "Castle Sansa - Tall Room Near Wheel Crawlers": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state),
            "Castle Sansa - Alcove Near Dungeon": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 1)
                or self.has_slide(state),
            "Castle Sansa - Balcony": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3)
                or self.has_plunge(state) and self.get_kicks(state, 1)
                or self.has_slide(state),
            "Castle Sansa - Corner Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3)
                or self.get_kicks(state, 2) and self.has_slide(state),
            "Castle Sansa - Wheel Crawlers": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.kick_or_plunge(state, 1)
                or self.has_slide(state),
            "Castle Sansa - Alcove Near Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 3)
                or self.has_slide(state) and self.kick_or_plunge(state, 1),
            "Castle Sansa - Near Theatre Front": lambda state:
                self.has_gem(state)
                or self.has_slide(state)
                or self.get_kicks(state, 4)
                or self.get_kicks(state, 2) and self.has_plunge(state),
            "Castle Sansa - High Climb From Courtyard": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state)
                or self.can_attack(state) and self.get_kicks(state, 1)
                or self.has_slide(state),
            "Listless Library - Sun Greaves": lambda state:
                self.can_attack(state),
            "Listless Library - Sun Greaves 1": lambda state:
                self.can_attack(state),
            "Listless Library - Sun Greaves 2": lambda state:
                self.can_attack(state),
            "Listless Library - Sun Greaves 3": lambda state:
                self.can_attack(state),
            "Listless Library - Upper Back": lambda state:
                (self.has_breaker(state) or self.knows_obscure(state) and self.has_plunge(state))
                and (
                    self.has_gem(state)
                    or self.kick_or_plunge(state, 2)
                    or self.has_slide(state)),
            "Listless Library - Locked Door Across": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 1)
                or self.has_slide(state),
            "Listless Library - Locked Door Left": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 2)
                or self.has_slide(state) and self.kick_or_plunge(state, 1),
        })

    def set_pseudoregalia_rules(self) -> None:
        super().set_pseudoregalia_rules()


class PseudoregaliaLunaticRules(PseudoregaliaRules):
    def __init__(self, world) -> None:
        super().__init__(world)

        self.region_rules.update({
            "Dungeon Mirror -> Dungeon Slide": lambda state:
                self.can_attack(state),
            "Dungeon Slide -> Dungeon Mirror": lambda state:
                self.can_attack(state),
            "Dungeon Slide -> Dungeon Strong Eyes": lambda state:
                self.has_slide(state),
            "Dungeon Slide -> Dungeon Escape Lower": lambda state:
                self.can_attack(state) and self.navigate_darkrooms(state),
            "Dungeon Strong Eyes -> Dungeon Slide": lambda state:
                self.has_slide(state),
            "Dungeon Strong Eyes -> Dungeon => Castle": lambda state:
                self.has_small_keys(state),
            "Dungeon => Castle -> Dungeon Strong Eyes": lambda state:
                self.has_small_keys(state),
            "Dungeon Escape Lower -> Dungeon Slide": lambda state:
                self.can_attack(state),
            "Dungeon Escape Lower -> Dungeon Escape Upper": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.kick_or_plunge(state, 2)
                or self.has_slide(state) and self.kick_or_plunge(state, 1),
            "Dungeon Escape Upper -> Theatre Outside Scythe Corridor": lambda state:
                self.can_bounce(state)
                or self.kick_or_plunge(state, 1)
                or self.has_gem(state)
                or self.has_slide(state),
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
            "Library Main -> Library Locked": lambda state:
                self.has_small_keys(state),
            "Library Main -> Library Greaves": lambda state:
                self.has_slide(state),
            "Library Main -> Library Top": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 2)
                or self.has_slide(state),
            "Library Greaves -> Library Top": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state),
            "Library Top -> Library Greaves": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 2)
                or self.can_bounce(state) and self.get_kicks(state, 1) and self.has_plunge(state)
        })

        self.location_rules.update({
            "Dilapidated Dungeon - Dark Orbs": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1) and self.can_bounce(state)
                or self.get_kicks(state, 3) and self.has_plunge(state)
                or self.has_slide(state) and self.get_kicks(state, 1)
                or self.has_slide(state) and self.can_bounce(state),
            "Dilapidated Dungeon - Past Poles": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 2)
                or self.has_slide(state) and self.get_kicks(state, 1) and self.has_plunge(state),
            "Dilapidated Dungeon - Rafters": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 2)
                or self.can_bounce(state) and self.kick_or_plunge(state, 1)
                or self.has_slide(state),
            "Dilapidated Dungeon - Strong Eyes": lambda state:
                self.has_breaker(state)
                or self.has_gem(state)
                or self.has_slide(state) and self.kick_or_plunge(state, 1),
            "Castle Sansa - Floater In Courtyard": lambda state:
                self.can_bounce(state)
                and (
                    self.kick_or_plunge(state, 1)
                    or self.has_slide(state))
                or self.has_slide(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 3)
                or self.has_gem(state),
            "Castle Sansa - Locked Door": lambda state:
                self.has_small_keys(state),
            "Castle Sansa - Platform In Main Halls": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 1)
                or self.has_slide(state)
                or self.can_bounce(state),
            "Castle Sansa - Tall Room Near Wheel Crawlers": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state),
            "Castle Sansa - Alcove Near Dungeon": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 1)
                or self.has_slide(state),
            "Castle Sansa - Balcony": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3)
                or self.has_plunge(state) and self.get_kicks(state, 1)
                or self.has_slide(state),
            "Castle Sansa - Corner Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3)
                or self.get_kicks(state, 1) and self.has_slide(state),
            "Castle Sansa - Wheel Crawlers": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.kick_or_plunge(state, 1)
                or self.has_slide(state),
            "Castle Sansa - Alcove Near Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 1),  # This never really matters and that makes me sad
            "Castle Sansa - Near Theatre Front": lambda state:
                self.has_gem(state)
                or self.has_slide(state)
                or self.get_kicks(state, 4)
                or self.get_kicks(state, 2) and self.has_plunge(state),
            "Castle Sansa - High Climb From Courtyard": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state)
                or self.can_attack(state) and self.get_kicks(state, 1)
                or self.has_slide(state),
            "Listless Library - Sun Greaves": lambda state:
                self.can_attack(state),
            "Listless Library - Sun Greaves 1": lambda state:
                self.can_attack(state),
            "Listless Library - Sun Greaves 2": lambda state:
                self.can_attack(state),
            "Listless Library - Sun Greaves 3": lambda state:
                self.can_attack(state),
            "Listless Library - Upper Back": lambda state:
                (self.has_breaker(state) or self.knows_obscure(state) and self.has_plunge(state))
                and (
                    self.has_gem(state)
                    or self.kick_or_plunge(state, 2)
                    or self.has_slide(state)),
            "Listless Library - Locked Door Across": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 1)
                or self.has_slide(state),
            "Listless Library - Locked Door Left": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 2)
                or self.has_slide(state) and self.kick_or_plunge(state, 1),
        })

    def set_pseudoregalia_rules(self) -> None:
        super().set_pseudoregalia_rules()
