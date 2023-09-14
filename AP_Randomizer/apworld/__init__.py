from worlds.AutoWorld import World
from .Items import PseudoregaliaItem, PseudoregaliaItemData, item_table
from worlds.generic.Rules import add_rule, set_rule, forbid_item
from .Rules import get_kicks, can_bounce


class PseudoregaliaWorld(World):
    game = "Pseudoregalia"

    def create_item(self, name: str) -> PseudoregaliaItem:
        data = item_table[name]
        return PseudoregaliaItem(name, data.classification, data.code, self.player)
