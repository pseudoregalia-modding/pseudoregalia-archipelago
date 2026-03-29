from BaseClasses import CollectionState
from typing import Dict, Callable, TYPE_CHECKING
from worlds.generic.Rules import add_rule, set_rule, CollectionRule
from .constants.difficulties import NORMAL, EXPERT, LUNATIC
from .constants.versions import MAP_PATCH
from .locations import location_table
from .options import PseudoregaliaOptions

from rule_builder.rules import Rule, And, Or, Has, HasAll, HasAllCounts, True_, False_
from typing_extensions import override
from dataclasses import dataclass
from .logic import OptionData, RuleData, RefRuleData, RegionData, LocationData, PseudoregaliaData

if TYPE_CHECKING:
    from . import PseudoregaliaWorld
else:
    PseudoregaliaWorld = object


class PseudoregaliaRulesHelpers:
    world: PseudoregaliaWorld
    player: int
    region_rules: dict[str, list[CollectionRule]]
    location_rules: dict[str, list[CollectionRule]]
    # Empty list or missing keys are True, any False rules need to be explicit, multiple rules are ORd together
    # Classes instantiated in difficulty order and append new clauses to rules,
    # add_rule applies them backwards meaning harder rules will shortcircuit easier rules

    required_small_keys: int = 6  # Set to 7 for Normal logic.
    knows_dungeon_escape: bool

    def __init__(self, world: PseudoregaliaWorld) -> None:
        self.world = world
        self.player = world.player
        self.region_rules = {}
        self.location_rules = {}

        # memoize functions that differ based on options
        if world.options.game_version == MAP_PATCH:
            self.can_gold_ultra = self.can_slidejump
            self.can_gold_slide_ultra = lambda state: False
        else:
            self.can_gold_ultra = self.has_slide
            self.can_gold_slide_ultra = self.has_slide

        # TODO convert knows_obscure to just a bool?
        if world.options.obscure_logic:
            self.knows_obscure = lambda state: True
            self.can_attack = lambda state: self.has_breaker(state) or self.has_plunge(state)
        else:
            self.knows_obscure = lambda state: False
            self.can_attack = self.has_breaker

        spawn_point = world.options.spawn_point
        dungeon_start = spawn_point == spawn_point.option_dungeon_mirror
        self.knows_dungeon_escape = dungeon_start or bool(world.options.obscure_logic)

        logic_level = world.options.logic_level.value
        if logic_level in (EXPERT, LUNATIC):
            self.navigate_darkrooms = lambda state: True
        elif self.knows_dungeon_escape:
            self.navigate_darkrooms = lambda state: state.has("Ascendant Light", self.player) or self.has_breaker(state)
        else:
            self.navigate_darkrooms = lambda state: state.has("Ascendant Light", self.player)

        if logic_level == NORMAL:
            self.required_small_keys = 7

    def apply_clauses(self, region_clauses, location_clauses):
        for name, rule in region_clauses.items():
            if name not in self.region_rules:
                self.region_rules[name] = []
            self.region_rules[name].append(rule)
        for name, rule in location_clauses.items():
            if name not in self.location_rules:
                self.location_rules[name] = []
            self.location_rules[name].append(rule)

    def has_breaker(self, state) -> bool:
        return state.has_any({"Dream Breaker", "Progressive Dream Breaker"}, self.player)

    def has_slide(self, state) -> bool:
        return state.has_any({"Slide", "Progressive Slide"}, self.player)

    def has_plunge(self, state) -> bool:
        return state.has("Sunsetter", self.player)

    def can_bounce(self, state) -> bool:
        return self.has_breaker(state) and state.has("Ascendant Light", self.player)

    def can_attack(self, state) -> bool:
        """Used where either breaker or sunsetter will work, for example on switches.
        Using sunsetter is considered Obscure Logic by this method."""
        raise Exception("can_attack() was not set")

    def get_kicks(self, state: CollectionState, count: int) -> bool:
        return state.has("Kick Count", self.player, count)

    def get_clings(self, state: CollectionState, count: int) -> bool:
        return state.has("Cling Count", self.player, count)

    def kick_or_plunge(self, state: CollectionState, count: int) -> bool:
        """Used where one air kick can be replaced with sunsetter.
        Input is the number of kicks needed without plunge."""
        total: int = state.count("Kick Count", self.player)
        if state.has("Sunsetter", self.player):
            total += 1
        return total >= count

    def has_small_keys(self, state) -> bool:
        if not self.can_attack(state):
            return False
        return state.count("Small Key", self.player) >= self.required_small_keys

    def navigate_darkrooms(self, state) -> bool:
        """Used on entry into the dungeon darkrooms."""
        raise Exception("navigate_darkrooms() was not set")

    def can_slidejump(self, state) -> bool:
        return (state.has_all({"Slide", "Solar Wind"}, self.player)
                or state.count("Progressive Slide", self.player) >= 2)

    def can_gold_ultra(self, state) -> bool:
        """Used when a gold ultra is needed and it is possible to solar ultra."""
        raise Exception("can_gold_ultra() was not set")

    def can_gold_slide_ultra(self, state) -> bool:
        """Used when a gold ultra is needed but it is not possible to solar ultra."""
        raise Exception("can_gold_slide_ultra() was not set")

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
        world = self.world
        multiworld = self.world.multiworld

        for name, rules in self.region_rules.items():
            entrance = multiworld.get_entrance(name, self.player)
            for index, rule in enumerate(rules):
                if index == 0:
                    set_rule(entrance, rule)
                else:
                    add_rule(entrance, rule, "or")
        for name, rules in self.location_rules.items():
            if not location_table[name].can_create(world.options):
                continue
            location = multiworld.get_location(name, self.player)
            for index, rule in enumerate(rules):
                if index == 0:
                    set_rule(location, rule)
                else:
                    add_rule(location, rule, "or")

        set_rule(multiworld.get_location("D S T RT ED M M O   Y", self.player), lambda state:
                 self.has_breaker(state) and state.has_all({
                     "Major Key - Empty Bailey",
                     "Major Key - The Underbelly",
                     "Major Key - Tower Remains",
                     "Major Key - Sansa Keep",
                     "Major Key - Twilight Theatre",
                 }, self.player))
        multiworld.completion_condition[self.player] = lambda state: state.has(
            "Something Worth Being Awake For", self.player)




def check_tags(player_tags: dict[str, int], tags: dict[str, int] | None) -> bool:
    return tags is None or all(level <= player_tags[tag] for tag, level in tags.items())

def check_options(player_options: PseudoregaliaOptions, options: OptionData | None,
                  resolution_if_none: bool = True) -> bool:
    if options is None:
        return resolution_if_none

    for option_name, value in options.items():
        option = getattr(player_options, option_name)
        if isinstance(value, bool):
            has_option = option == value
        else:
            has_option = option == getattr(option.__class__, f"option_{value}")
        if not has_option:
            return False
    return True

class PseudoregaliaRule(Rule[PseudoregaliaWorld], game="Pseudoregalia"):
    rule: Rule
    tags: dict[str, int] | None
    options: OptionData | None

    def __init__(self, rule_data: RuleData, ref_rules: dict[str, "PseudoregaliaRule"]):
        self.tags = rule_data.tags
        self.options = rule_data.options

        if rule_data.and_ is not None:
            self.rule = And(*(PseudoregaliaRule(child_rule_data, ref_rules) for child_rule_data in rule_data.and_))
        elif rule_data.or_ is not None:
            self.rule = Or(*(PseudoregaliaRule(child_rule_data, ref_rules) for child_rule_data in rule_data.or_))
        elif rule_data.has is not None:
            if isinstance(rule_data.has, str):
                self.rule = Has(rule_data.has)
            elif isinstance(rule_data.has, list):
                self.rule = HasAll(*rule_data.has)
            else:
                self.rule = HasAllCounts(rule_data.has)
        elif rule_data.ref is not None:
            self.rule = ref_rules[rule_data.ref]
        else:
            self.rule = True_()

    @override
    def _instantiate(self, world: PseudoregaliaWorld) -> Rule.Resolved:
        passes_filter = check_tags(world.tags) and check_options(world.options)
        return self.rule.resolve(world) if passes_filter else False_().resolve(world)

def create_entrance_name(start: str, end: str, entrance_name: str | None) -> str:
    return entrance_name if entrance_name is not None else f"{start} -> {end}"

@dataclass
class PseudoregaliaRules:
    entrance_rules: dict[str, PseudoregaliaRule]
    location_rules: dict[str, PseudoregaliaRule]
    completion_rule: PseudoregaliaRule

    def get_entrance_rule(self, start: str, end: str, entrance_name: str | None) -> PseudoregaliaRule | None:
        """Helper to resolve entrance name and get rule if it exists."""
        return self.entrance_rules.get(create_entrance_name(start, end, entrance_name))

def create_rules(pseudoregalia_data: PseudoregaliaData) -> PseudoregaliaRules:
    ref_rules: dict[str, PseudoregaliaRule] = {}
    for ref_rule_data in pseudoregalia_data.ref_rules:
        ref_rules[ref_rule_data.name] = PseudoregaliaRule(ref_rule_data.rule, ref_rules)

    entrance_rules: dict[str, PseudoregaliaRule] = {}
    for region_data in pseudoregalia_data.regions:
        if region_data.exits is None:
            continue
        for exit_data in region_data.exits:
            if exit_data.rule is None:
                continue
            entrance_name = create_entrance_name(region_data.name, exit_data.region, exit_data.entrance_name)
            entrance_rules[entrance_name] = PseudoregaliaRule(exit_data.rule, ref_rules)

    location_rules: dict[str, PseudoregaliaRule] = {}
    for location_data in pseudoregalia_data.locations:
        if location_data.rule is None:
            continue
        location_rules[location_data.name] = PseudoregaliaRule(location_data.rule, ref_rules)

    return PseudoregaliaRules(
        entrance_rules,
        location_rules,
        PseudoregaliaRule(pseudoregalia_data.completion_rule, ref_rules),
    )
