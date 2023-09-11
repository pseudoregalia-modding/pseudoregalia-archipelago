from BaseClasses import Item, ItemClassification
from typing import NamedTuple


class PseudoregaliaItem(Item):
    game = "Pseudoregalia"


class PseudoregaliaItemData(NamedTuple):
    code: int = None
    classification: ItemClassification = ItemClassification.filler
