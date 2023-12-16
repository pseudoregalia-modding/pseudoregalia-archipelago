from BaseClasses import CollectionState
from typing import Dict, Callable, TYPE_CHECKING
from worlds.generic.Rules import set_rule
from .constants.difficulties import NORMAL

if TYPE_CHECKING:
    from . import PseudoregaliaWorld
else:
    PseudoregaliaWorld = object


class PseudoregaliaRulesHelpers:
    world: PseudoregaliaWorld
    player: int
    region_rules: Dict[str, Callable[[CollectionState], bool]]
    location_rules: Dict[str, Callable[[CollectionState], bool]]
    required_small_keys: int = 6  # Set to 7 for Normal logic.

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
            "Tower Remains -> Underbelly Little Guy": lambda state:
                self.has_plunge(state),
            "Tower Remains -> The Great Door": lambda state:
                self.has_gem(state) and self.get_kicks(state, 3),
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
        """Used where either breaker or sunsetter will work, for example on switches.
        Using sunsetter is considered Obscure Logic by this method."""
        raise Exception("can_attack() was not set")

    def get_kicks(self, state, count: int) -> bool:
        kicks: int = 0
        if (state.has("Sun Greaves", self.player)):
            kicks += 3
        kicks += state.count("Heliacal Power", self.player)
        kicks += state.count("Air Kick", self.player)
        return kicks >= count

    def kick_or_plunge(self, state, count: int) -> bool:
        """Used where one air kick can be replaced with sunsetter.
        Input is the number of kicks needed without plunge."""
        total: int = 0
        if (state.has("Sun Greaves", self.player)):
            total += 3
        if (state.has("Sunsetter", self.player)):
            total += 1
        total += state.count("Heliacal Power", self.player)
        total += state.count("Air Kick", self.player)
        return total >= count

    def has_small_keys(self, state) -> bool:
        if not self.can_attack(state):
            return False
        return state.count("Small Key", self.player) >= self.required_small_keys

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
        """True when Obscure Logic is enabled, False when it isn't."""
        raise Exception("knows_obscure() was not set")

    def set_pseudoregalia_rules(self) -> None:
        multiworld = self.world.multiworld
        split_kicks = bool(self.world.multiworld.split_sun_greaves[self.player])
        if bool(self.world.multiworld.obscure_logic[self.player]):
            self.knows_obscure = lambda state: True
            self.can_attack = lambda state: self.has_breaker(state) or self.has_plunge(state)
        else:
            self.knows_obscure = lambda state: False
            self.can_attack = lambda state: self.has_breaker(state)

        logic_level = self.multiworld.logic_level[self.player].value
        if logic_level == NORMAL:
            self.required_small_keys = 7

        for name, rule in self.region_rules.items():
            entrance = multiworld.get_entrance(name, self.player)
            set_rule(entrance, rule)
        for name, rule in self.location_rules.items():
            if name.startswith("Listless Library"):
                if split_kicks and name.endswith("Greaves"):
                    continue
                if not split_kicks and name[-1].isdigit():
                    continue
            location = multiworld.get_location(name, self.player)
            set_rule(location, rule)

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
