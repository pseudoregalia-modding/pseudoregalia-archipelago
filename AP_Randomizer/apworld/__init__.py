from typing import List
from worlds.AutoWorld import World
from .Items import PseudoregaliaItem, PseudoregaliaItemData, item_data_table
from .Locations import PseudoregaliaLocation, location_data_table
from .Regions import create_regions


class PseudoregaliaWorld(World):
    """goate"""

    game = "Pseudoregalia"

    location_name_to_id = {name: data.code for name, data in location_data_table.items()}
    item_name_to_id = {name: data.code for name, data in item_data_table.items()}

    def create_items(self):
        item_pool: List[PseudoregaliaItem] = []
        for name, data in item_data_table.items():
            item_pool += [self.create_item(name)]
        self.multiworld.itempool += item_pool

    def create_item(self, name: str) -> PseudoregaliaItem:
        data = item_data_table[name]
        return PseudoregaliaItem(name, data.classification, data.code, self.player)

    def create_regions(self):
        create_regions(self.multiworld, self.player)
