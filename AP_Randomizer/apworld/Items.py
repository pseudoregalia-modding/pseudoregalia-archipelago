from BaseClasses import Item, ItemClassification
from typing import NamedTuple, Dict


class PseudoregaliaItem(Item):
    game = "Pseudoregalia"


class PseudoregaliaItemData(NamedTuple):
    code: int = None
    classification: ItemClassification = ItemClassification.filler


item_table: Dict[str, PseudoregaliaItemData] = {
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
    "Soul Cutter": PseudoregaliaItemData(
        code=2365810010,
        classification=ItemClassification.progression),

    "Heliacal Power": PseudoregaliaItemData(
        code=2365810011,
        classification=ItemClassification.progression),
    "Aerial Finesse": PseudoregaliaItemData(
        code=2365810012,
        classification=ItemClassification.filler),
    "Pilgrimage": PseudoregaliaItemData(
        code=2365810013,
        classification=ItemClassification.filler),
    "Empathy": PseudoregaliaItemData(
        code=2365810014,
        classification=ItemClassification.filler),
    "Good Graces": PseudoregaliaItemData(
        code=2365810015,
        classification=ItemClassification.useful),
    "Martial Prowess": PseudoregaliaItemData(
        code=2365810016,
        classification=ItemClassification.useful),
    "Clear Mind": PseudoregaliaItemData(
        code=2365810017,
        classification=ItemClassification.filler),
    "The Professional": PseudoregaliaItemData(
        code=2365810018,
        classification=ItemClassification.filler),

    # "Health Piece": PseudoregaliaItemData(
    # code=2365810019,
    # classification=ItemClassification.useful),
    "Small Key": PseudoregaliaItemData(
        code=2365810020,
        classification=ItemClassification.progression),

    "Major Key - Empty Bailey": PseudoregaliaItemData(
        code=2365810021,
        classification=ItemClassification.progression),
    "Major Key - The Underbelly": PseudoregaliaItemData(
        code=2365810022,
        classification=ItemClassification.progression),
    "Major Key - Tower Remains": PseudoregaliaItemData(
        code=2365810023,
        classification=ItemClassification.progression),
    "Major Key - Sansa Keep": PseudoregaliaItemData(
        code=2365810024,
        classification=ItemClassification.progression),
    "Major Key - Twilight Theatre": PseudoregaliaItemData(
        code=2365810025,
        classification=ItemClassification.progression),
}

item_frequencies = {
    "Empathy": 2,
    "Good Graces": 2,
    "Clear Mind": 3,
    "Small Key": 7,
}
