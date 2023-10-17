from worlds.AutoWorld import World
from BaseClasses import Region, Location, Item
from .items import PseudoregaliaItem, PseudoregaliaItemData, item_table, item_frequencies, item_groups
from .locations import location_table
from .regions import region_table
from .options import PseudoregaliaOptions
from worlds.generic.Rules import add_rule, set_rule, forbid_item
from .rules import PseudoregaliaRules, PseudoregaliaNormalRules, PseudoregaliaHardRules, PseudoregaliaExpertRules, PseudoregaliaLunaticRules
from typing import Dict, Any
from .constants.difficulties import UNIVERSAL, NORMAL, HARD, EXPERT, LUNATIC


class PseudoregaliaWorld(World):
    game = "Pseudoregalia"
    required_client_version = (0, 5, 0)

    item_name_to_id = {name: data.code for name, data in item_table.items() if data.code is not None}
    location_name_to_id = {name: data.code for name, data in location_table.items() if data.code is not None}
    locked_locations = {name: data for name, data in location_table.items() if data.locked_item}
    item_name_groups = item_groups

    options_dataclass = PseudoregaliaOptions
    options: PseudoregaliaOptions

    def create_item(self, name: str) -> PseudoregaliaItem:
        data = item_table[name]
        return PseudoregaliaItem(name, data.classification, data.code, self.player)

    def create_items(self):
        for item_name, item_data in item_table.items():
            if (item_name == "Dream Breaker"):
                continue  # Really skrunkled way of just adding the one locked breaker to the pool for now.
            if (item_data.code is None):
                continue
            if (item_name in item_frequencies):
                for count in range(item_frequencies[item_name]):
                    self.multiworld.itempool.append(
                        Item(item_name, item_data.classification, item_data.code, self.player))
            else:
                self.multiworld.itempool.append(Item(item_name, item_data.classification, item_data.code, self.player))

    def create_regions(self):
        for region_name in region_table.keys():
            self.multiworld.regions.append(Region(region_name, self.player, self.multiworld))

        for loc_name, loc_data in location_table.items():
            region = self.multiworld.get_region(loc_data.region, self.player)
            new_loc = Location(self.player, loc_name, loc_data.code, region)
            if (not loc_data.show_in_spoiler):
                new_loc.show_in_spoiler = False
            region.locations.append(new_loc)

        for region_name, exit_list in region_table.items():
            region = self.multiworld.get_region(region_name, self.player)
            region.add_exits(exit_list)

        # Place locked locations.
        for location_name, location_data in self.locked_locations.items():
            locked_item = self.create_item(location_table[location_name].locked_item)
            self.multiworld.get_location(location_name, self.player).place_locked_item(locked_item)

    def fill_slot_data(self) -> Dict[str, Any]:
        return {"slot_number": self.player,
                "death_link": self.options.death_link.value,
                "difficulty": self.options.logic_level.current_key,
                "obscure": self.options.obscure_tricks.value, }

    def set_rules(self):
        difficulty = self.options.logic_level
        if difficulty == NORMAL:
            PseudoregaliaNormalRules(self).set_pseudoregalia_rules()
        elif difficulty == HARD:
            PseudoregaliaHardRules(self).set_pseudoregalia_rules()
        elif difficulty == EXPERT:
            PseudoregaliaExpertRules(self).set_pseudoregalia_rules()
        elif difficulty == LUNATIC:
            PseudoregaliaLunaticRules(self).set_pseudoregalia_rules()
