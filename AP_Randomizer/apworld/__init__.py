from worlds.AutoWorld import World
from .Items import PseudoregaliaItem, PseudoregaliaItemData, item_table
from worlds.generic.Rules import add_rule, set_rule, forbid_item
from .Rules import get_kicks, can_bounce


class PseudoregaliaWorld(World):
    game = "Pseudoregalia"

    def create_item(self, name: str) -> PseudoregaliaItem:
        data = item_table[name]
        return PseudoregaliaItem(name, data.classification, data.code, self.player)

    def set_rules(self):
        set_rule(self.multiworld.get_location("Dungeon - HP By Breaker", self.player),
                 lambda state:
                 any([
                     can_bounce(state, self.player),
                     get_kicks(state, self.player) == 3,
                     state.has("Sunsetter", self.player)
                 ]))
