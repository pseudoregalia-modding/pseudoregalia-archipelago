from collections.abc import Callable
from typing import NamedTuple

from BaseClasses import Item, ItemClassification

from .constants.versions import MAP_PATCH
from .options import PseudoregaliaOptions


class PseudoregaliaItem(Item):
    game = "Pseudoregalia"


class PseudoregaliaItemData(NamedTuple):
    code: int | None = None
    frequency: int = 1
    classification: ItemClassification = ItemClassification.filler
    precollect: Callable[[PseudoregaliaOptions], int] = lambda options: 0
    can_create: Callable[[PseudoregaliaOptions], bool] = lambda options: True


def precollect_if_theatre_start(precollect_if_normal: bool) -> Callable[[PseudoregaliaOptions], int]:
    def precollect(options: PseudoregaliaOptions) -> int:
        is_theatre_start = options.spawn_point == "theatre_main"
        is_normal = options.logic_level == options.logic_level.option_normal
        matches_difficulty = precollect_if_normal == is_normal
        return 1 if is_theatre_start and matches_difficulty else 0
    return precollect


precollect_if_theatre_start_normal = precollect_if_theatre_start(True)
precollect_if_theatre_start_hard_plus = precollect_if_theatre_start(False)


item_table: dict[str, PseudoregaliaItemData] = {
    "Dream Breaker": PseudoregaliaItemData(
        code=1,
        classification=ItemClassification.progression,
        precollect=lambda options: 1 if options.start_with_breaker else 0,
        can_create=lambda options: not bool(options.progressive_breaker)),
    "Indignation": PseudoregaliaItemData(
        code=2,
        classification=ItemClassification.useful),
    "Sun Greaves": PseudoregaliaItemData(
        code=3,
        classification=ItemClassification.progression,
        can_create=lambda options: not bool(options.split_sun_greaves)),
    "Slide": PseudoregaliaItemData(
        code=4,
        classification=ItemClassification.progression,
        can_create=lambda options: not bool(options.progressive_slide)),
    "Solar Wind": PseudoregaliaItemData(
        code=5,
        classification=ItemClassification.progression,
        can_create=lambda options: not bool(options.progressive_slide)),
    "Sunsetter": PseudoregaliaItemData(
        code=6,
        classification=ItemClassification.progression),
    "Strikebreak": PseudoregaliaItemData(
        code=7,
        classification=ItemClassification.progression,
        can_create=lambda options: not bool(options.progressive_breaker)),
    "Cling Gem": PseudoregaliaItemData(
        code=8,
        classification=ItemClassification.progression,
        precollect=precollect_if_theatre_start_normal,
        can_create=lambda options: not options.split_cling_gem),
    "Ascendant Light": PseudoregaliaItemData(
        code=9,
        classification=ItemClassification.progression),
    "Soul Cutter": PseudoregaliaItemData(
        code=10,
        classification=ItemClassification.progression,
        can_create=lambda options: not bool(options.progressive_breaker)),

    "Heliacal Power": PseudoregaliaItemData(
        code=11,
        classification=ItemClassification.progression,
        precollect=precollect_if_theatre_start_hard_plus,
        can_create=lambda options: not bool(options.split_sun_greaves)),
    "Aerial Finesse": PseudoregaliaItemData(
        code=12,
        classification=ItemClassification.filler),
    "Pilgrimage": PseudoregaliaItemData(
        code=13,
        classification=ItemClassification.filler),
    "Empathy": PseudoregaliaItemData(
        code=14,
        frequency=2,
        classification=ItemClassification.filler),
    "Good Graces": PseudoregaliaItemData(
        code=15,
        frequency=2,
        classification=ItemClassification.useful),
    "Martial Prowess": PseudoregaliaItemData(
        code=16,
        classification=ItemClassification.useful),
    "Clear Mind": PseudoregaliaItemData(
        code=17,
        frequency=3,
        classification=ItemClassification.filler),
    "Professionalism": PseudoregaliaItemData(
        code=18,
        precollect=lambda options: 1 if options.game_version == MAP_PATCH and not options.randomize_time_trials else 0,
        classification=ItemClassification.filler),

    "Health Piece": PseudoregaliaItemData(
        code=19,
        frequency=16,
        classification=ItemClassification.useful),
    "Small Key": PseudoregaliaItemData(
        code=20,
        frequency=7,
        classification=ItemClassification.progression),

    "Major Key - Empty Bailey": PseudoregaliaItemData(
        code=21,
        classification=ItemClassification.progression),
    "Major Key - The Underbelly": PseudoregaliaItemData(
        code=22,
        classification=ItemClassification.progression),
    "Major Key - Tower Remains": PseudoregaliaItemData(
        code=23,
        classification=ItemClassification.progression),
    "Major Key - Sansa Keep": PseudoregaliaItemData(
        code=24,
        classification=ItemClassification.progression),
    "Major Key - Twilight Theatre": PseudoregaliaItemData(
        code=25,
        classification=ItemClassification.progression),

    "Progressive Slide": PseudoregaliaItemData(
        code=26,
        frequency=2,
        classification=ItemClassification.progression,
        can_create=lambda options: bool(options.progressive_slide)),
    "Air Kick": PseudoregaliaItemData(
        code=27,
        frequency=4,
        classification=ItemClassification.progression,
        precollect=precollect_if_theatre_start_hard_plus,
        can_create=lambda options: bool(options.split_sun_greaves)),
    "Progressive Dream Breaker": PseudoregaliaItemData(
        code=28,
        frequency=3,
        classification=ItemClassification.progression,
        precollect=lambda options: 1 if options.start_with_breaker else 0,
        can_create=lambda options: bool(options.progressive_breaker)),

    "Devotion": PseudoregaliaItemData(
        code=29,
        classification=ItemClassification.filler,
        precollect=lambda options: 1 if not options.randomize_time_trials else 0,
        can_create=lambda options: options.game_version == MAP_PATCH),
    "A Guardian": PseudoregaliaItemData(
        code=30,
        classification=ItemClassification.filler,
        precollect=lambda options: 1 if not options.randomize_time_trials else 0,
        can_create=lambda options: options.game_version == MAP_PATCH),
    "Sweater": PseudoregaliaItemData(
        code=31,
        classification=ItemClassification.filler,
        precollect=lambda options: 1 if not options.randomize_time_trials else 0,
        can_create=lambda options: options.game_version == MAP_PATCH),
    "Class": PseudoregaliaItemData(
        code=32,
        classification=ItemClassification.filler,
        precollect=lambda options: 1 if not options.randomize_time_trials else 0,
        can_create=lambda options: options.game_version == MAP_PATCH),
    "Chivalry": PseudoregaliaItemData(
        code=33,
        classification=ItemClassification.filler,
        precollect=lambda options: 1 if not options.randomize_time_trials else 0,
        can_create=lambda options: options.game_version == MAP_PATCH),
    "Nostalgia": PseudoregaliaItemData(
        code=34,
        classification=ItemClassification.filler,
        precollect=lambda options: 1 if not options.randomize_time_trials else 0,
        can_create=lambda options: options.game_version == MAP_PATCH),
    "A Bleeding Heart": PseudoregaliaItemData(
        code=35,
        classification=ItemClassification.filler,
        precollect=lambda options: 1 if not options.randomize_time_trials else 0,
        can_create=lambda options: options.game_version == MAP_PATCH),

    "Memento": PseudoregaliaItemData(
        code=36,
        classification=ItemClassification.useful,
        precollect=lambda options: 1 if options.start_with_map else 0,
        can_create=lambda options: options.game_version == MAP_PATCH),

    "Cling Shard": PseudoregaliaItemData(
        code=37,
        classification=ItemClassification.progression,
        frequency=3,
        precollect=precollect_if_theatre_start_normal,
        can_create=lambda options: bool(options.split_cling_gem),
    ),

    "Healing": PseudoregaliaItemData(
        code=38,
        frequency=0,
        classification=ItemClassification.filler,
    ),
    "Magic Power": PseudoregaliaItemData(
        code=39,
        frequency=0,
        classification=ItemClassification.filler,
    ),

    "Something Worth Being Awake For": PseudoregaliaItemData(
        classification=ItemClassification.progression),
}

item_groups: dict[str, set[str]] = {
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
    # some nice to have groups when sorting local/non local items in yaml etc,
    # does not include "Memento" aka new map powerup
    "aspects": {"Indignation",
                "Aerial Finesse",
                "Pilgrimage",
                "Empathy",
                "Martial Prowess",
                "Clear Mind",
                "Professionalism",
                "Good Graces"},
    "mobility": {"Sun Greaves",
                 "Slide",
                 "Solar Wind",
                 "Ascendant Light",
                 "Heliacal Power",
                 "Progressive Slide",
                 "Sunsetter",
                 "Air Kick",
                 "Cling Gem"},
    "collectables": {"Health Piece",
                     "Small Key"},
    # "weapon": {"Dream Breaker",
    #            "Progressive Dream Breaker",
    #            "Strikebreak",
    #            "Soul Cutter"},
    # "attire": {"Professional", # Castle Sansa trial
    #            "Soldier", # Empty Bailey trial
    #            "Guardian", # Sansa Keep trial
    #            "Sol Sister", # Dilapidated Dungeon trial
    #            "Classy", # Twilight Theatre trial
    #            "XIX", # Underbelly trial
    #            "Sleepytime", # Listless Library trial
    #            "Bleeding Heart}, # Tower Remains trial
}
