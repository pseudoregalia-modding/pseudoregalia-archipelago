from BaseClasses import Item, ItemClassification, MultiWorld
from typing import NamedTuple, Dict, Set, Callable


class PseudoregaliaItem(Item):
    game = "Pseudoregalia"


class PseudoregaliaItemData(NamedTuple):
    code: int = None
    classification: ItemClassification = ItemClassification.filler
    can_create: Callable[[MultiWorld, int], bool] = lambda multiworld, player: True


item_table: Dict[str, PseudoregaliaItemData] = {
    "Dream Breaker": PseudoregaliaItemData(
        code=2365810001,
        classification=ItemClassification.progression,
        can_create=lambda multiworld, player: not bool(multiworld.progressive_breaker[player])),
    "Indignation": PseudoregaliaItemData(
        code=2365810002,
        classification=ItemClassification.useful),
    "Sun Greaves": PseudoregaliaItemData(
        code=2365810003,
        classification=ItemClassification.progression,
        can_create=lambda multiworld, player: not bool(multiworld.split_sun_greaves[player])),
    "Slide": PseudoregaliaItemData(
        code=2365810004,
        classification=ItemClassification.progression,
        can_create=lambda multiworld, player: not bool(multiworld.progressive_slide[player])),
    "Solar Wind": PseudoregaliaItemData(
        code=2365810005,
        classification=ItemClassification.progression,
        can_create=lambda multiworld, player: not bool(multiworld.progressive_slide[player])),
    "Sunsetter": PseudoregaliaItemData(
        code=2365810006,
        classification=ItemClassification.progression),
    "Strikebreak": PseudoregaliaItemData(
        code=2365810007,
        classification=ItemClassification.progression,
        can_create=lambda multiworld, player: not bool(multiworld.progressive_breaker[player])),
    "Cling Gem": PseudoregaliaItemData(
        code=2365810008,
        classification=ItemClassification.progression),
    "Ascendant Light": PseudoregaliaItemData(
        code=2365810009,
        classification=ItemClassification.progression),
    "Soul Cutter": PseudoregaliaItemData(
        code=2365810010,
        classification=ItemClassification.progression,
        can_create=lambda multiworld, player: not bool(multiworld.progressive_breaker[player])),

    "Heliacal Power": PseudoregaliaItemData(
        code=2365810011,
        classification=ItemClassification.progression,
        can_create=lambda multiworld, player: not bool(multiworld.split_sun_greaves[player])),
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
    "Professionalism": PseudoregaliaItemData(
        code=2365810018,
        classification=ItemClassification.filler),

    "Health Piece": PseudoregaliaItemData(
        code=2365810019,
        classification=ItemClassification.useful),
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

    "Progressive Slide": PseudoregaliaItemData(
        code=2365810026,
        classification=ItemClassification.progression,
        can_create=lambda multiworld, player: bool(multiworld.progressive_slide[player])),
    "Air Kick": PseudoregaliaItemData(
        code=2365810027,
        classification=ItemClassification.progression,
        can_create=lambda multiworld, player: bool(multiworld.split_sun_greaves[player])),
    "Progressive Dream Breaker": PseudoregaliaItemData(
        code=2365810028,
        classification=ItemClassification.progression,
        can_create=lambda multiworld, player: bool(multiworld.progressive_breaker[player])),

    "Unlocked Door": PseudoregaliaItemData(
        classification=ItemClassification.useful),

    "Something Worth Being Awake For": PseudoregaliaItemData(
        classification=ItemClassification.progression),
}

item_frequencies = {
    "Empathy": 2,
    "Good Graces": 2,
    "Clear Mind": 3,
    "Small Key": 7,
    "Health Piece": 16,
    "Progressive Slide": 2,
    "Air Kick": 4,
}

item_groups: Dict[str, Set[str]] = {
    "major keys": {"Major Key - Empty Bailey",
                   "Major Key - The Underbelly",
                   "Major Key - Tower Remains",
                   "Major Key - Sansa Keep",
                   "Major Key - Twilight Theatre"},
    "plunge": {"Sunsetter"},
    "air kicks": {"Sun Greaves"},
    "nike kicks": {"Sun Greaves"},
    "charge": {"Strikebreak"},
    "projectile": {"Soul Cutter"},
    "slidejump": {"Solar Wind"},
    "wallride": {"Cling Gem"},
    "pogo": {"Ascendant Light"},
    "floof": {"Professionalism"},
    "heliacal power": {"Air Kick"},
}
