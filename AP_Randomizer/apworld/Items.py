from BaseClasses import Item, ItemClassification, MultiWorld
from typing import Callable, Dict, NamedTuple, Optional


class PseudoregaliaItem(Item):
    game = "Pseudoregalia"


class PseudoregaliaItemData(NamedTuple):
    code: Optional[int] = None
    type: ItemClassification = ItemClassification.filler
    can_create: Callable[[MultiWorld, int], bool] = lambda multiworld, player: True


# Pseudoregalia's steam id is 2365810 so these ids will just start with 2365810000 for convention
item_data_table: Dict[str, PseudoregaliaItemData] = {
    "Dream Breaker": PseudoregaliaItemData(
        code=2365810001,
        type=ItemClassification.progression),
    "Indignation": PseudoregaliaItemData(
        code=2365810002,
        type=ItemClassification.useful),
    "Sun Greaves": PseudoregaliaItemData(
        code=2365810003,
        type=ItemClassification.progression),
    "Slide": PseudoregaliaItemData(
        code=2365810004,
        type=ItemClassification.progression),
    "Solar Wind": PseudoregaliaItemData(
        code=2365810005,
        type=ItemClassification.progression),
    "Sunsetter": PseudoregaliaItemData(
        code=2365810006,
        type=ItemClassification.progression),
    "Strikebreak": PseudoregaliaItemData(
        code=2365810007,
        type=ItemClassification.progression),
    "Cling Gem": PseudoregaliaItemData(
        code=2365810008,
        type=ItemClassification.progression),
    "Ascendant Light": PseudoregaliaItemData(
        code=2365810009,
        type=ItemClassification.progression),
    "Soul Cutter": PseudoregaliaItemData(
        code=2365810010,
        type=ItemClassification.progression),
    "Clear Mind": PseudoregaliaItemData(
        code=2365810011,
        type=ItemClassification.useful),
    "Empathy": PseudoregaliaItemData(
        code=2365810012,
        type=ItemClassification.useful),
    "Good Graces": PseudoregaliaItemData(
        code=2365810013,
        type=ItemClassification.useful),
    "Martial Prowess": PseudoregaliaItemData(
        code=2365810014,
        type=ItemClassification.useful),
    "Pilgrimage": PseudoregaliaItemData(
        code=2365810015,
        type=ItemClassification.useful),
    "Aerial Finesse": PseudoregaliaItemData(
        code=2365810016,
        type=ItemClassification.useful),
    "Heliacal Power": PseudoregaliaItemData(
        code=2365810017,
        type=ItemClassification.progression),
    "Professionalism": PseudoregaliaItemData(
        code=2365810018,
        type=ItemClassification.filler),
    "Health Piece": PseudoregaliaItemData(
        code=2365810019,
        type=ItemClassification.useful),
    "Small Key": PseudoregaliaItemData(
        code=2365810020,
        type=ItemClassification.progression),
    "Major Key": PseudoregaliaItemData(
        code=2365810021,
        type=ItemClassification.progression),
}

item_frequencies = {
    "Clear Mind": 3,
    "Empathy": 2,
    "Good Graces": 2,
}

item_table = {name: data.code for name, data in item_data_table.items() if data.code is not None}
