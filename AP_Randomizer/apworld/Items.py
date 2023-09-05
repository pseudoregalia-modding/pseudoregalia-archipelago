from BaseClasses import Item, ItemClassification, MultiWorld
from typing import Callable, Dict, NamedTuple, Optional


class PseudoregaliaItem(Item):
    game = "Pseudoregalia"


class PseudoregaliaItemData(NamedTuple):
    code: Optional[int] = None
    classification: ItemClassification = ItemClassification.filler
    can_create: Callable[[MultiWorld, int], bool] = lambda multiworld, player: True


# Pseudoregalia's steam id is 2365810 so these ids will just start with 2365810000 for convention
item_data_table: Dict[str, PseudoregaliaItemData] = {
    "Dream Breaker": PseudoregaliaItemData(
        code=2365810001,
        classification=ItemClassification.progression),
    "Indignation": PseudoregaliaItemData(
        code=2365810002,
        classification=ItemClassification.useful),
    "Sun Greaves": PseudoregaliaItemData(
        code=2365810003,
        classification=ItemClassification.progression),
    "Slide": PseudoregaliaItemData(
        code=2365810004,
        classification=ItemClassification.progression),
    "Solar Wind": PseudoregaliaItemData(
        code=2365810005,
        classification=ItemClassification.progression),
    "Sunsetter": PseudoregaliaItemData(
        code=2365810006,
        classification=ItemClassification.progression),
    "Strikebreak": PseudoregaliaItemData(
        code=2365810007,
        classification=ItemClassification.progression),
    "Cling Gem": PseudoregaliaItemData(
        code=2365810008,
        classification=ItemClassification.progression),
    "Ascendant Light": PseudoregaliaItemData(
        code=2365810009,
        classification=ItemClassification.progression),
    """
    "Clear Mind": PseudoregaliaItemData(
        code=2365810011,
        classification=ItemClassification.useful),
    "Empathy": PseudoregaliaItemData(
        code=2365810012,
        classification=ItemClassification.useful),
    "Good Graces": PseudoregaliaItemData(
        code=2365810013,
        classification=ItemClassification.useful),
    "Martial Prowess": PseudoregaliaItemData(
        code=2365810014,
        classification=ItemClassification.useful),
    "Pilgrimage": PseudoregaliaItemData(
        code=2365810015,
        classification=ItemClassification.useful),
    "Aerial Finesse": PseudoregaliaItemData(
        code=2365810016,
        classification=ItemClassification.useful),
    "Heliacal Power": PseudoregaliaItemData(
        code=2365810017,
        classification=ItemClassification.progression),
    "Professionalism": PseudoregaliaItemData(
        code=2365810018,
        classification=ItemClassification.filler),
    "Health Piece": PseudoregaliaItemData(
        code=2365810019,
        classification=ItemClassification.useful),
    "Small Key": PseudoregaliaItemData(
        code=2365810020,
        classification=ItemClassification.progression),
        """
    # Moved soul cutter down here temporarily because otherwise the block comment throws an error
    "Soul Cutter": PseudoregaliaItemData(
        code=2365810010,
        classification=ItemClassification.progression),
}

item_table = {name: data.code for name, data in item_data_table.items() if data.code is not None}
