from BaseClasses import Item, ItemClassification


class PseudoregaliaItem(Item):
    game = "Pseudoregalia"
    code: int = None
    classification: ItemClassification = ItemClassification.filler
