from worlds.AutoWorld import World
from BaseClasses import Region, Location
from .Items import PseudoregaliaItem, PseudoregaliaItemData, item_table
from .Locations import location_table
from .Regions import region_table
from worlds.generic.Rules import add_rule, set_rule, forbid_item
from .Rules import get_kicks, can_bounce


class PseudoregaliaWorld(World):
    game = "Pseudoregalia"

    item_name_to_id = {name: data.code for name, data in item_table.items()}
    location_name_to_id = {name: data.code for name, data in location_table.items()}

    def create_item(self, name: str) -> PseudoregaliaItem:
        data = item_table[name]
        return PseudoregaliaItem(name, data.classification, data.code, self.player)

    def create_regions(self):
        for region_name in region_table.keys():
            self.multiworld.regions.append(Region(region_name, self.player, self.multiworld))

        for loc_name, loc_data in location_table.items():
            region = self.multiworld.get_region(loc_data.region, self.player)
            region.locations.append(Location(self.player, loc_name, loc_data.code, region))

        for region_name, exit_list in region_table.items():
            region = self.multiworld.get_region(region_name, self.player)

            for exit in exit_list:
                connecting_region = self.multiworld.get_region(exit.region, self.player)
                region.connect(connecting_region, None, exit.access_rule if exit.access_rule else None)
