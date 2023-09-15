from typing import NamedTuple, Callable, Dict, List, Optional
from BaseClasses import CollectionState
from Rules import has_breaker, can_bounce, get_kicks, has_small_keys, navigate_darkrooms


class RegionExit(NamedTuple):
    region: str
    access_rule: Callable[[CollectionState, int], bool] = lambda state, player: True
    breakable_wall: bool = False


region_table: Dict[str, List[RegionExit]] = {
    "Menu": [],
    "Dungeon Mirror": [],
    "Dungeon Strong Eyes": [],
    "Castle Sansa": [],
    "Library Main": [],
    "Library Locked": [],
    "Keep Main": [],
    "Keep Sunsetter": [],
    "Empty Bailey": [],
    "Tower Remains": [],
    "Underbelly Hole": [],  # I don't actually know what this is.
    "Underbelly Main": [],
    "Theatre Pillar": [],
    "Theatre Main": [],
    "The Great Door": [],
}
