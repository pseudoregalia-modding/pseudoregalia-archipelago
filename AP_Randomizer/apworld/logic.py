from __future__ import annotations
from dataclasses import dataclass
from typing import TypeAlias

@dataclass
class PseudoregaliaData:
    item_mapping: dict[str, str | list[str] | ItemMappingData]
    """
    Defines how to map progression items to pseudo items in world collect and remove functions.
    
    A str value is a simple mapping, so one of this item maps to one of the pseudo item.
    A list[str] value is for progressive items: the first of this item maps to the pseudo item at index 0, the second
      maps to the pseudo item at index 1, etc.
    An ItemMappingData value is for cases where one of the item maps to more than one of the pseudo item and/or there is
      a max to how many of the item can contribute to the pseudo item.
    """
    tags: list[TagData]
    """Defines the tags that can be used to filter rules."""
    tag_groups: list[TagGroupData]
    """
    Defines groups that bundle tags together to help make customization simpler.
    
    Entries in this list can contain tags as well as other tag groups as children. Child relationships should not form
    cycles, but for documentation purpose, this list should be sorted from more general to less general. To validate
    this, groups should only reference other groups as children that are defined below them in the list.
    """
    ref_rules: list[RefRuleData]
    """
    Defines some common rules that can be referenced in any other rule.
    
    Entries in this list can even reference each other, but they should not form cycles. To validate this, references in
    this list can only refer to rules defined above them.
    """
    regions: list[RegionData]
    """Defines the world's regions and entrances."""
    origins: list[OriginData]
    """Defines origin data, which is used for setting the world origin_region_name field based on the SpawnPoint option."""
    locations: list[LocationData]
    """Defines the world's locations."""
    completion_rule: RuleData
    """Defines the completion condition for the world."""

@dataclass
class ItemMappingData:
    name: str
    """The name of the pseudo item to map to."""
    count: int | None
    """The amount of the pseudo item that one of this item maps to. None defaults to 1."""
    max: int | None
    """The max number of this item that can contribute to the pseudo item. None means no limit."""

@dataclass
class TagData:
    """
    Defines a tag. At least one of the fields corresponding to a logic level must be non-None. If more than one is
    defined, the description must also be non-None. Otherwise the description for the one level serves as the
    description for the tag as a whole.
    """

    name: str
    """The name of the tag. Must be unique across all tags."""
    description: str | None
    """The tag description. Must be defined if there is more than one level in the tag. Used for documentation only."""
    advanced: str | None
    """Description for what the tag does at logic level 1."""
    hard: str | None
    """Description for what the tag does at logic level 2."""
    expert: str | None
    """Description for what the tag does at logic level 3."""
    lunatic: str | None
    """Description for what the tag does at logic level 4."""

@dataclass
class TagGroupData:
    name: str
    """The name of the tag group."""
    description: str
    """A short description of the tag group. Not used by the apworld but for generating documentation."""
    children: list[str]
    """A list of tags and tag groups that are children of this tag group."""

@dataclass
class RefRuleData:
    name: str
    rule: RuleData

@dataclass
class RegionData:
    name: str
    """The name of the region. Must be unique across all regions."""
    exits: list[ExitData] | None
    """A list of all entrances starting from this region."""

@dataclass
class OriginData:
    spawn_point: str
    """
    The SpawnPoint option corresponding to this origin without the option_ prefix. Must be unique across all origins.
    """
    region: str
    """The region associated with this origin. Must be the name of a region in the regions list."""

@dataclass
class LocationData:
    """Defines data for locations and events. Exactly one of code and event_item must be not None."""
    name: str
    """The name of the location. Must be unique across all locations."""
    code: int | None
    """The location's code. Must be unique across all locations if defined."""
    region: str
    """The region to put the location in. Must be the name of a region in the regions list."""
    rule: RuleData | None
    """The rule for this location."""
    can_create: OptionData | None
    """Provides a set of options to determined whether the location should be created."""
    event_item: str | None
    """Name of the even item to lock to this location."""

@dataclass
class RuleData:
    """
    A rule on an entrance or location. Aside from the tags field, at most one field can be not None. If all fields are
    None, the rule is built using the True_ RuleBuilder object.
    
    Fields can be added to support other RuleBuilder objects as needed.
    """
    and_: list[RuleData] | None
    """Maps to the And RuleBuilder object."""
    or_: list[RuleData] | None
    """Maps to the Or RuleBuilder object."""
    has: str | list[str] | dict[str, int] | None
    """
    Maps to the Has, HasAll, or HasAllCounts RuleBuilder objects depending on the type.
    
    All string values in this type must be valid item names or the names of pseudo items in item_mapping.
    """
    can_reach_region: str | None
    """Maps to the CanReachRegion RuleBuilder object."""
    ref: str | None
    """Maps to a ref_rule by name."""
    tags: dict[str, int] | None
    """Provides filtering. The keys are required tags and the values are the minimum logic level for the tag."""
    options: OptionData | None
    """Provides additional filtering based on other options."""

@dataclass
class ExitData:
    region: str
    """The region this entrance exits to. Must be the name of a region in the regions list."""
    entrance_name: str | None
    """Overrides the default entrance name. If None, the default of '{from} -> {to}' is used."""
    rule: RuleData | None
    """The rule for this entrance."""

OptionData: TypeAlias = dict[str, bool | str]
"""
Represents a set of options and required values.

Keys in the dict correspond to attributes in the PseudoregaliaOptions class. If the option is a Toggle, the value should
be a bool, and if the option is a Choice, the value should be a str. A str value should be an option field in that
option's class without the option_ prefix. For example, this dict could contain "game_version: map_patch".

During generation, this can be resolved to a boolean by checking the player's options. It resovles to True iff the value
of each option is equal to the value in the dict.
"""
