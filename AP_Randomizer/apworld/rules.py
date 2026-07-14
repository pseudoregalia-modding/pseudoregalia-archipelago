from dataclasses import dataclass
from typing import TYPE_CHECKING
from typing_extensions import override

from rule_builder.rules import Rule, And, Or, Has, HasAll, HasAllCounts, CanReachRegion, True_, False_

from .logic import OptionData, RuleData, PseudoregaliaData
from .options import PseudoregaliaOptions

if TYPE_CHECKING:
    from . import PseudoregaliaWorld
else:
    PseudoregaliaWorld = object


def check_tags(player_tags: dict[str, int], tags: dict[str, int] | None) -> bool:
    return tags is None or all(level <= player_tags[tag] for tag, level in tags.items())

def check_options(player_options: PseudoregaliaOptions, options: OptionData | None) -> bool:
    return options is None or all(getattr(player_options, op_name) == value for op_name, value in options.items())

class PseudoregaliaRule(Rule[PseudoregaliaWorld], game="Pseudoregalia"):
    rule: Rule
    tags: dict[str, int] | None
    option_data: OptionData | None

    def __init__(self, rule_data: RuleData, ref_rules: dict[str, "PseudoregaliaRule"]):
        self.tags = rule_data.tags
        self.option_data = rule_data.options

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
        elif rule_data.can_reach_region is not None:
            self.rule = CanReachRegion(rule_data.can_reach_region)
        elif rule_data.ref is not None:
            self.rule = ref_rules[rule_data.ref]
        else:
            self.rule = True_()

    @override
    def _instantiate(self, world: PseudoregaliaWorld) -> Rule.Resolved:
        passes_filter = check_tags(world.tags, self.tags) and check_options(world.options, self.option_data)
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
