from .Rules import get_encouragement_rule
from .Regions import region_data_table
from .Locations import PseudoregaliaLocation, location_data_table, location_table
from .Items import PseudoregaliaItem, item_data_table, item_table
from worlds.AutoWorld import World
from BaseClasses import Region
from typing import List
import sys
sys.path.append("D:/Projects/Pseudoregalia_Modding/MyMods/AP_Randomizer/_apworld_files/archipelago")


class PseudoregaliaWorld(World):
    """goate"""

    game = "Pseudoregalia"

    location_name_to_id = location_table
    item_name_to_id = item_table

    def create_item(self, name: str) -> PseudoregaliaItem:
        print("PSEUDOREGALIA creating", name)
        return PseudoregaliaItem(
            name, item_data_table[name].type, item_data_table[name].code, self.player)

    def create_items(self) -> None:
        print("PSEUDOREGALIA begin item creation??")
        item_pool: List[PseudoregaliaItem] = []
        for name, item in item_data_table.items():
            if item.code and item.can_create(self.multiworld, self.player):
                item_pool.append(self.create_item(name))

        self.multiworld.itempool += item_pool

    def create_regions(self) -> None:
        # Create regions.
        for region_name in region_data_table.keys():
            region = Region(region_name, self.player, self.multiworld)
            self.multiworld.regions.append(region)

        # Create locations.
        for region_name, region_data in region_data_table.items():
            region = self.multiworld.get_region(region_name, self.player)
            region.add_locations({
                location_name: location_data.address for location_name, location_data in location_data_table.items()
                if location_data.region == region_name and location_data.can_create(self.multiworld, self.player)
            }, PseudoregaliaLocation)
            print
            region.add_exits(region_data_table[region_name].connecting_regions)

        # Set priority location for Encouragement.
        # self.multiworld.priority_locations[self.player].value.add("The End")

    def set_rules(self) -> None:
        encouragement_rule = get_encouragement_rule(
            self.multiworld, self.player)
        self.multiworld.get_location(
            "The End", self.player).access_rule = encouragement_rule

        # Completion condition.
        self.multiworld.completion_condition[self.player] = lambda state: state.has(
            "Encouragement", self.player)
