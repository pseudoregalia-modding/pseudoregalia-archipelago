from __future__ import annotations
from typing import Literal

from BaseClasses import ItemClassification
from Options import Choice, Toggle

from .items import item_table
from .logic import ExitData, ItemMappingData, LocationData, OptionData, OriginData, PseudoregaliaData, RefRuleData, \
    RegionData, RuleData, TagGroupData, TagData
from .options import PseudoregaliaOptions, SpawnPoint
from .rules import create_entrance_name

# TODO: improve validation_context when context_type="key" by using this regex
# if it matches, use the f".{key}" format
# otherwise, use the f'["{key}"]' format
# escape quotes?? probably not necessary but would be "correct"
# r"^[a-zA-Z_][a-zA-Z0-9_]*$"u

class Validator:
    def validation_context(*, context_type: Literal["start", "key", "index"], key: str | None = None):
        """
        Manages the path for validation errors. Keeping track of the path helps to describe exactly where in the data
        the error occured.
        
        Context type `"start"` should be used only at the start of validation. Context type `"key"` should be used when
        descending into an object. Context type `"index"` should be used when descending into an array.
        
        If `context_type="key"` and `key=None` or if `context_type="index"`, the first argument after the validator will
        be added to the path and should be a `str` or `int` respectively.
        """
        def decorator(func):
            def wrapper(validator: Validator, *args, **kwargs):
                if context_type == "start":
                    path_part = "$"
                elif context_type == "key":
                    path_part = f".{key if key is not None else args[0]}"
                elif context_type == "index":
                    path_part = f"[{args[0]}]"

                validator.path_parts.append(path_part)
                func(validator, *args, **kwargs)
                validator.path_parts.pop()
            return wrapper
        return decorator

    path_parts: list[str]
    errors: list[str]
    pseudo_items: set[str]
    tags: dict[str, set[int]]
    tag_groups: set[str]
    ref_rules: set[str]
    regions: set[str]
    entrances: set[str]
    spawn_points: set[str]
    locations: set[str]
    location_codes: set[int]
    # the regions field is for verifying unique region names, but we need the names of all regions before that is
    # finished, so we pre-compute region names and put them here
    all_regions: set[str]

    def __init__(self):
        self.path_parts = []
        self.errors = []
        self.pseudo_items = set()
        self.tags = {}
        self.tag_groups = set()
        self.ref_rules = set()
        self.regions = set()
        self.entrances = set()
        self.spawn_points = set()
        self.locations = set()
        self.location_codes = set()
        self.all_regions = set()

    def err(self, msg: str):
        path = "".join(self.path_parts)
        self.errors.append(f"{path}: {msg}")

    @validation_context(context_type="start")
    def validate_data(self, data: PseudoregaliaData):
        self.all_regions.update(region.name for region in data.regions)
        self.validate_item_mapping(data.item_mapping)
        self.validate_tags(data.tags)
        self.validate_tag_groups(data.tag_groups)
        self.validate_ref_rules(data.ref_rules)
        self.validate_regions(data.regions)
        self.validate_origins(data.origins)
        self.validate_locations(data.locations)
        self.validate_completion_rule(data.completion_rule)

    @validation_context(context_type="key", key="item_mapping")
    def validate_item_mapping(self, item_mapping: dict[str, str | list[str] | ItemMappingData]):
        for item_name, mapping in item_mapping.items():
            self.validate_item_mapping_entry(item_name, mapping)

    @validation_context(context_type="key")
    def validate_item_mapping_entry(self, item_name: str, mapping: str | list[str] | ItemMappingData):
        # CHECK: keys in item_mapping match the name of a progression item
        if not self.is_progression(item_name):
            self.err("item referenced by key does not exist or is not marked as progression")

        if isinstance(mapping, str):
            self.pseudo_items.add(mapping)
        elif isinstance(mapping, list):
            self.pseudo_items.update(mapping)
        elif isinstance(mapping, ItemMappingData):
            self.pseudo_items.update(mapping.names)

    @validation_context(context_type="key", key="tags")
    def validate_tags(self, tags: list[TagData]):
        for i, tag_data in enumerate(tags):
            self.validate_tag(i, tag_data)

    @validation_context(context_type="index")
    def validate_tag(self, index: int, tag_data: TagData):
        return_early = self.validate_tag_name(tag_data.name)
        if return_early:
            return

        self.tags[tag_data.name] = set()
        if tag_data.advanced:
            self.tags[tag_data.name].add(1)
        if tag_data.hard:
            self.tags[tag_data.name].add(2)
        if tag_data.expert:
            self.tags[tag_data.name].add(3)
        if tag_data.lunatic:
            self.tags[tag_data.name].add(4)

        difficulties = len(self.tags[tag_data.name])
        # CHECK: tags have at least one of advanced/hard/expert/lunatic
        if difficulties == 0:
            self.err("tag has no difficulty descriptions")
        # CHECK: tags with exactly one of advanced/hard/expert/lunatic don't have description
        elif difficulties == 1 and tag_data.description:
            self.err("tag has one difficulty description and a tag description")
        # CHECK: tags with more than one of advanced/hard/expert/lunatic have description
        elif difficulties > 1 and not tag_data.description:
            self.err("tag has more than one difficulty description but no tag description")

    @validation_context(context_type="key", key="name")
    def validate_tag_name(self, name: str) -> bool:
        # CHECK: tag names are unique
        if name in self.tags:
            self.err("not unique across tags")
            return True
        return False

    @validation_context(context_type="key", key="tag_groups")
    def validate_tag_groups(self, tag_groups: list[TagGroupData]):
        for i, tag_group_data in enumerate(tag_groups):
            self.validate_tag_group(i, tag_group_data)

    @validation_context(context_type="index")
    def validate_tag_group(self, index: int, tag_group_data: TagGroupData):
        self.validate_tag_group_name(tag_group_data.name)
        self.validate_tag_group_children(tag_group_data.children)

        # track this after checking children to make sure the group doesn't have itself as a child
        self.tag_groups.add(tag_group_data.name)

    @validation_context(context_type="key", key="name")
    def validate_tag_group_name(self, name: str):
        # CHECK: tag group names are unique
        if name in self.tag_groups:
            self.err("not unique across tag groups")

    @validation_context(context_type="key", key="children")
    def validate_tag_group_children(self, children: list[str]):
        for i, child in enumerate(children):
            self.validate_tag_groupy_child(i, child)

    @validation_context(context_type="index")
    def validate_tag_groupy_child(self, index: int, child: str):
        # CHECK: tag group children are tags or other tag groups, and they don't have cycles
        if child not in self.tags and child not in self.tag_groups:
            self.err("does not match a tag or previously defined tag group")

    @validation_context(context_type="key", key="ref_rules")
    def validate_ref_rules(self, ref_rules: list[RefRuleData]):
        for i, ref_rule_data in enumerate(ref_rules):
            self.validate_ref_rule(i, ref_rule_data)

    @validation_context(context_type="index")
    def validate_ref_rule(self, index: int, ref_rule_data: RefRuleData):
        self.validate_ref_rule_name(ref_rule_data.name)
        self.validate_rule(ref_rule_data.rule)

        # track this after checking rule to make sure the ref rule doesn't references itself
        self.ref_rules.add(ref_rule_data.name)

    @validation_context(context_type="key", key="name")
    def validate_ref_rule_name(self, name: str):
        # CHECK: ref rule names are unique
        if name in self.ref_rules:
            self.err("not unique across ref rules")

    @validation_context(context_type="key", key="regions")
    def validate_regions(self, regions: list[RegionData]):
        for i, region_data in enumerate(regions):
            self.validate_region(i, region_data)

    @validation_context(context_type="index")
    def validate_region(self, index: int, region_data: RegionData):
        self.validate_region_name(region_data.name)
        if region_data.exits is not None:
            self.validate_region_exits(region_data.name, region_data.exits)

    @validation_context(context_type="key", key="name")
    def validate_region_name(self, name: str):
        # CHECK: region names are unique
        if name in self.regions:
            self.err("not unique across regions")
        else:
            self.regions.add(name)

    @validation_context(context_type="key", key="exits")
    def validate_region_exits(self, region_name: str, exits: list[ExitData]):
        for i, exit_data in enumerate(exits):
            self.validate_region_exit(i, region_name, exit_data)

    @validation_context(context_type="index")
    def validate_region_exit(self, index: int, region_name: str, exit_data: ExitData):
        self.validate_region_exit_region(exit_data.region)
        entrance_name = create_entrance_name(region_name, exit_data.region, exit_data.entrance_name)            
        self.validate_region_exit_entrance_name(entrance_name)
        if exit_data.rule is not None:
            self.validate_rule(exit_data.rule)

    @validation_context(context_type="key", key="region")
    def validate_region_exit_region(self, region: str):
        # CHECK: region matches the name of a region
        if region not in self.all_regions:
            self.err("does not match the name of a region")

    @validation_context(context_type="key", key="entrance_name")
    def validate_region_exit_entrance_name(self, entrance_name: str):
        # CHECK: entrance names are unique
        if entrance_name in self.entrances:
            self.err("not unique across entrances")
        else:
            self.entrances.add(entrance_name)

    @validation_context(context_type="key", key="origins")
    def validate_origins(self, origins: list[OriginData]):
        for i, origin_data in enumerate(origins):
            self.validate_origin(i, origin_data)

    @validation_context(context_type="index")
    def validate_origin(self, index: int, origin_data: OriginData):
        self.validate_origin_spawn_point(origin_data.spawn_point)
        self.validate_origin_region(origin_data.region)

    @validation_context(context_type="key", key="spawn_point")
    def validate_origin_spawn_point(self, spawn_point: str):
        # CHECK: origin spawn points are unique
        if spawn_point in self.spawn_points:
            self.err("not unique across origins")
        else:
            self.spawn_points.add(spawn_point)

        attr_name = f"option_{spawn_point}"
        # CHECK: origin names match an attribute on the SpawnPoint object
        if not hasattr(SpawnPoint, attr_name):
            self.err("SpawnPoint has no option with this name")

    @validation_context(context_type="key", key="region")
    def validate_origin_region(self, region: str):
        # CHECK: origin regions match existing regions
        if region not in self.all_regions:
            self.err("does not match the name of a region")

    @validation_context(context_type="key", key="locations")
    def validate_locations(self, locations: list[LocationData]):
        for i, location_data in enumerate(locations):
            self.validate_location(i, location_data)

    @validation_context(context_type="index")
    def validate_location(self, index: int, location_data: LocationData):
        # CHECK: locations have exactly one of {code, event_item} defined
        if location_data.code is None and location_data.event_item is None:
            self.err("neither of {code, event_item} is defined")
        elif location_data.code is not None and location_data.event_item is not None:
            self.err("both of {code, event_item} are defined")

        self.validate_location_name(location_data.name)
        if location_data.code is not None:
            self.validate_location_code(location_data.code)
        self.validate_location_region(location_data.region)
        if location_data.rule is not None:
            self.validate_rule(location_data.rule)
        if location_data.can_create is not None:
            self.validate_location_can_create(location_data.can_create)
        if location_data.event_item is not None:
            self.validate_location_event_item(location_data.event_item)

    @validation_context(context_type="key", key="name")
    def validate_location_name(self, name: str):
        # CHECK: location names are unique
        if name in self.locations:
            self.err("not unique across locations")
        else:
            self.locations.add(name)

    @validation_context(context_type="key", key="code")
    def validate_location_code(self, code: int):
        # CHECK: location codes are unique
        if code in self.location_codes:
            self.err("not unique across locations")
        else:
            self.location_codes.add(code)

    @validation_context(context_type="key", key="region")
    def validate_location_region(self, region: str):
        # CHECK: location regions match existing regions
        if region not in self.all_regions:
            self.err("does not match the name of a region")

    @validation_context(context_type="key", key="can_create")
    def validate_location_can_create(self, options: OptionData):
        self.check_options(options)

    @validation_context(context_type="key", key="event_item")
    def validate_location_event_item(self, item: str):
        # CHECK: event items match the name of a progression item
        if not self.is_progression(item):
            self.err("item does not exist or is not marked as progression")
            return

        # CHECK: event items don't have a code
        if item_table[item].code is not None:
            self.err("item has a code")

    @validation_context(context_type="key", key="completion_rule")
    def validate_completion_rule(self, rule: RuleData):
        self.check_rule(rule)

    @validation_context(context_type="key", key="rule")
    def validate_rule(self, rule: RuleData):
        self.check_rule(rule)

    def check_rule(self, rule: RuleData):
        rule_types = 0
        if rule.and_ is not None:
            self.validate_rule_and(rule.and_)
            rule_types += 1
        if rule.or_ is not None:
            self.validate_rule_or(rule.or_)
            rule_types += 1
        if rule.has is not None:
            self.validate_rule_has(rule.has)
            rule_types += 1
        if rule.can_reach_region is not None:
            self.validate_rule_can_reach_region(rule.can_reach_region)
            rule_types += 1
        if rule.ref is not None:
            self.validate_rule_ref(rule.ref)
            rule_types += 1

        # CHECK: at most one rule type is defined
        if rule_types > 1:
            self.err("more than one rule type defined")

        if rule.tags is not None:
            self.validate_rule_tags(rule.tags)
        if rule.options is not None:
            self.validate_rule_options(rule.options)

    @validation_context(context_type="key", key="and")
    def validate_rule_and(self, sub_rules: list[RuleData]):
        for i, sub_rule in enumerate(sub_rules):
            self.validate_rule_list_entry(i, sub_rule)

    @validation_context(context_type="key", key="or")
    def validate_rule_or(self, sub_rules: list[RuleData]):
        for i, sub_rule in enumerate(sub_rules):
            self.validate_rule_list_entry(i, sub_rule)

    @validation_context(context_type="index")
    def validate_rule_list_entry(self, index: int, rule: RuleData):
        self.check_rule(rule)

    @validation_context(context_type="key", key="has")
    def validate_rule_has(self, has: str | list[str] | dict[str, int]):
        if isinstance(has, str):
            self.check_has_item(has)
        elif isinstance(has, list):
            for i, item in enumerate(has):
                self.validate_has_item_entry(i, item)
        elif isinstance(has, dict):
            for item in has:
                self.validate_has_item_key(item)

    def check_has_item(self, item: str):
        if not self.is_progression(item) and item not in self.pseudo_items:
            self.err("does not match the name of a progression item or a pseudo item defined in item_mapping")

    @validation_context(context_type="index")
    def validate_has_item_entry(self, index: int, item: str):
        self.check_has_item(item)

    @validation_context(context_type="key")
    def validate_has_item_key(self, key: str):
        self.check_has_item(key)

    @validation_context(context_type="key", key="can_reach_region")
    def validate_rule_can_reach_region(self, region: str):
        if region not in self.all_regions:
            self.err("does not match the name of a region")

    @validation_context(context_type="key", key="ref")
    def validate_rule_ref(self, ref: str):
        if ref not in self.ref_rules:
            self.err("does not match the name of a ref rule")

    @validation_context(context_type="key", key="tags")
    def validate_rule_tags(self, tags: dict[str, int]):
        for tag_name, tag_level in tags.items():
            self.validate_rule_tag(tag_name, tag_level)

    @validation_context(context_type="key")
    def validate_rule_tag(self, name: str, level: int):
        # CHECK: name matches a tag
        if name not in self.tags:
            self.err("key does not match the name of a tag")
            # return since we can't check tag levels of a tag that doesn't exist
            return

        # CHECK: value matches a defined level for that tag
        if level not in self.tags[name]:
            self.err("value does not match a level defined for the tag")

    @validation_context(context_type="key", key="options")
    def validate_rule_options(self, options: OptionData):
        self.check_options(options)

    def check_options(self, options: OptionData):
        for name, value in options.items():
            self.validate_option(name, value)

    @validation_context(context_type="key")
    def validate_option(self, name: str, value: bool | str):
        # CHECK: key corresponds to an option in PseudoregaliaOptions
        if name not in PseudoregaliaOptions.__annotations__:
            self.err("key does not match an option in PseudoregaliaOptions")
            return

        option_class = PseudoregaliaOptions.__annotations__[name]
        if isinstance(value, bool):
            # CHECK: bool values corresponds to Toggle options
            if not issubclass(option_class, Toggle):
                self.err("value is a bool but option is not a Toggle")
        elif isinstance(value, str):
            # CHECK: str values correspond to Choice options
            if not issubclass(option_class, Choice):
                self.err("value is a str but option is not a Choice")
                return

            attr_name = f"option_{value}"
            # CHECK: str values match an attribute on the Choice option object
            if not hasattr(option_class, attr_name):
                self.err("option class has no option with this name")

    def is_progression(self, item: str) -> bool:
        return item in item_table and item_table[item].classification & ItemClassification.progression != 0
