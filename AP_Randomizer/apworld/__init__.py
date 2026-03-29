import pkgutil
from typing import Any
import yaml

from worlds.AutoWorld import World, WebWorld
from BaseClasses import Region, CollectionState, Tutorial

from .items import PseudoregaliaItem, item_table, item_groups
from .locations import PseudoregaliaLocation, location_table, zones
from .options import PseudoregaliaOptions
from .constants.difficulties import EXPERT, LUNATIC
from .constants.versions import FULL_GOLD
from .logic import ItemMappingData, PseudoregaliaData
from .dacite import from_dict, Config
from .rules import create_rules, check_options


yaml_bytes = pkgutil.get_data(__name__, "logic.yaml")
yaml_dict = yaml.safe_load(yaml_bytes)
config = Config(
    strict=True,
    strict_unions_match=True,
    # we append an underscore to fields that would be reserved otherwise, so we have to undo that with convert_key
    convert_key=lambda key: key[:-1] if key.endswith("_") else key,
)
pseudoregalia_data = from_dict(PseudoregaliaData, yaml_dict, config)
pseudoregalia_rules = create_rules(pseudoregalia_data)


class PseudoregaliaWebWorld(WebWorld):
    setup_en = Tutorial(
        "Setup Guide",
        "A guide for setting up Pseudoregalia to be played in Archipelago.",
        "English",
        "setup_en.md",
        "setup/en",
        ["highrow623"]
    )
    tutorials = [setup_en]


class PseudoregaliaWorld(World):
    """
    Pseudoregalia is a wide-open 3D metroidvania with plenty of cool movement abilities to find and a sprawling
    dream-like castle to explore.
    """

    game = "Pseudoregalia"
    required_client_version = (0, 7, 0)
 
    item_name_to_id = {name: data.code for name, data in item_table.items() if data.code is not None}
    location_name_to_id = {name: data.code for name, data in location_table.items() if data.code is not None}
    item_name_groups = item_groups

    options_dataclass = PseudoregaliaOptions
    options: PseudoregaliaOptions

    web = PseudoregaliaWebWorld()

    filler = ("Healing", "Magic Power")
    filler_index = 0

    tags: dict[str, int] = {}

    def create_key_hints(self) -> Any:
        key_hints = [[] for _ in range(5)]
        key_locations = self.multiworld.find_items_in_locations(set(item_groups["major keys"]), self.player, True)
        for location in key_locations:
            if not location.item or not location.item.code or not location.address:
                # guard against optional fields being None
                continue
            # this implementation assumes major key item codes are all in a row starting at 2365810021 and will
            # break if that ever changes
            index = location.item.code - 2365810021
            if index not in range(5):
                # guard against index being out of bounds
                continue
            key_hints[index].append({
                "player": location.player,
                "location": location.address,
            })
        return key_hints

    # world overrides

    def get_filler_item_name(self) -> str:
        filler_item_name = self.filler[self.filler_index]
        self.filler_index = (self.filler_index + 1) % len(self.filler)
        return filler_item_name

    def create_item(self, name: str) -> PseudoregaliaItem:
        data = item_table[name]
        return PseudoregaliaItem(name, data.classification, data.code, self.player)

    def collect(self, state: CollectionState, item: PseudoregaliaItem) -> bool:
        # this function waits to call super().collect() until after the item mapping so that the mapping logic is done
        # in a similar state to remove(), i.e. mapping happens before the item is collected here but after the item is
        # removed in remove()
        if item.name not in pseudoregalia_data.item_mapping:
            return super().collect(state, item)

        mapping = pseudoregalia_data.item_mapping[item.name]
        if isinstance(mapping, str):
            state.add_item(mapping, self.player)
        elif isinstance(mapping, list):
            index = state.count(item.name, self.player)
            if index < len(mapping):
                state.add_item(mapping[index], self.player)
        elif isinstance(mapping, ItemMappingData):
            if mapping.max is None or state.count(item.name, self.player) < mapping.max:
                count = mapping.count if mapping.count is not None else 1
                state.add_item(mapping.name, self.player, count)
        return super().collect(state, item)

    def remove(self, state: CollectionState, item: PseudoregaliaItem) -> bool:
        ret = super().remove(state, item)
        if item.name not in pseudoregalia_data.item_mapping:
            return ret

        mapping = pseudoregalia_data.item_mapping[item.name]
        if isinstance(mapping, str):
            state.remove_item(mapping, self.player)
        elif isinstance(mapping, list):
            index = state.count(item.name, self.player)
            if index < len(mapping):
                state.remove_item(mapping[index], self.player)
        elif isinstance(mapping, ItemMappingData):
            if mapping.max is None or state.count(item.name, self.player) < mapping.max:
                count = mapping.count if mapping.count is not None else 1
                state.remove_item(mapping.name, self.player, count)
        return ret

    # generation overrides

    def generate_early(self):
        if self.options.logic_level in (EXPERT, LUNATIC):
            # obscure is forced on for expert/lunatic difficulties
            self.options.obscure_logic.value = 1
        if self.options.game_version == FULL_GOLD:
            # zero out options that don't do anything on full gold
            self.options.start_with_map.value = 0
            self.options.randomize_time_trials.value = 0
        spawn_point = self.options.spawn_point
        if spawn_point == spawn_point.option_dungeon_mirror:
            # start_with_breaker is forced on for dungeon start to help with sphere 1 size
            self.options.start_with_breaker.value = 1
        elif spawn_point == spawn_point.option_library:
            if not self.options.start_with_breaker and not self.options.randomize_books:
                # start_with_breaker is forced on if otherwise player wouldn't have enough checks
                self.options.start_with_breaker.value = 1

        # TODO fill out self.tags based on player options

    def create_regions(self):
        for origin_data in pseudoregalia_data.origins:
            if check_options(self.options, {"spawn_point": origin_data.spawn_point}):
                self.origin_region_name = origin_data.region
                break

        for region_data in pseudoregalia_data.regions:
            self.multiworld.regions.append(Region(region_data.name, self.player, self.multiworld))

        locations = sorted(pseudoregalia_data.locations, key=lambda loc_data: zones.index(loc_data.name.split(" - ")[0]))
        for loc_data in locations:
            if not check_options(self.options, loc_data.can_create):
                continue
            region = self.get_region(loc_data.region)
            new_loc = PseudoregaliaLocation(self.player, loc_data.name, loc_data.code, region)
            region.locations.append(new_loc)
            rule = pseudoregalia_rules.location_rules.get(loc_data.name)
            if rule is not None:
                self.set_rule(new_loc, rule)
            if loc_data.event_item:
                new_loc.place_locked_item(self.create_item(loc_data.event_item))

        for region_data in pseudoregalia_data.regions:
            if region_data.exits is None:
                continue
            region = self.get_region(region_data.name)
            for exit_data in region_data.exits:
                exit_region = self.get_region(exit_data.region)
                rule = pseudoregalia_rules.get_entrance_rule(region.name, exit_region.name, exit_data.entrance_name)
                self.create_entrance(region, exit_region, rule, exit_data.entrance_name)

    def create_items(self):
        itempool = []
        for item_name, item_data in item_table.items():
            if not item_data.can_create(self.options) or not item_data.code:
                continue
            precollect = item_data.precollect(self.options)
            for _ in range(precollect):
                self.multiworld.push_precollected(self.create_item(item_name))
            itempool += [self.create_item(item_name) for _ in range(precollect, item_data.frequency)]
        total_locations = len(self.multiworld.get_unfilled_locations(self.player))
        itempool += [self.create_filler() for _ in range(total_locations - len(itempool))]
        self.multiworld.itempool += itempool

    def set_rules(self):
        self.set_completion_rule(pseudoregalia_rules.completion_rule)

    def fill_slot_data(self) -> dict[str, Any]:
        slot_data = {
            "apworld_version": self.world_version,
            "game_version": self.options.game_version.value,
            "logic_level": self.options.logic_level.value,
            "spawn_point": self.options.spawn_point.value,
            "obscure_logic": bool(self.options.obscure_logic),
            "progressive_breaker": bool(self.options.progressive_breaker),
            "progressive_slide": bool(self.options.progressive_slide),
            "split_sun_greaves": bool(self.options.split_sun_greaves),
            "split_cling_gem": bool(self.options.split_cling_gem),
            "randomize_time_trials": bool(self.options.randomize_time_trials),
            "randomize_goats": bool(self.options.randomize_goats),
            "randomize_chairs": bool(self.options.randomize_chairs),
            "randomize_books": bool(self.options.randomize_books),
            "randomize_notes": bool(self.options.randomize_notes),
        }
        if self.options.major_key_hints:
            slot_data["key_hints"] = self.create_key_hints()
        return slot_data
