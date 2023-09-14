from worlds.AutoWorld import World
from .Items import PseudoregaliaItem, PseudoregaliaItemData, item_table
from .Locations import location_table
from worlds.generic.Rules import add_rule, set_rule, forbid_item
from .Rules import get_kicks, can_bounce


class PseudoregaliaWorld(World):
    game = "Pseudoregalia"

    item_name_to_id = {name: data.code for name, data in item_table.items()}
    location_name_to_id = {name: data.code for name, data in location_table.items()}

    def create_item(self, name: str) -> PseudoregaliaItem:
        data = item_table[name]
        return PseudoregaliaItem(name, data.classification, data.code, self.player)
