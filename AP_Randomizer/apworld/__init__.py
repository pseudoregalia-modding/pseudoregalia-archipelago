from worlds.AutoWorld import World
from .Items import PseudoregaliaItem, PseudoregaliaItemData, item_table


class PseudoregaliaWorld(World):
    game = "Pseudoregalia"

    def create_item(self, name: str) -> PseudoregaliaItem:
        data = item_table[name]
        return PseudoregaliaItem(name, data.classification, data.code, self.player)
