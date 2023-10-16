from typing import NamedTuple, Callable, Dict, List, Optional
from BaseClasses import CollectionState


class RegionExit(NamedTuple):
    region: str
    access_rule: Callable[[CollectionState, int], bool] = lambda state, player: True
    breakable_wall: bool = False


region_table: Dict[str, List[str]] = {
    "Menu": ["Dungeon Mirror"],
    "Dungeon Mirror": ["Dungeon Strong Eyes", "Underbelly Main", "Theatre Main",],
    "Dungeon Strong Eyes": ["Castle Main"],
    "Castle Main": ["Library Main", "Keep Main", "Empty Bailey", "Theatre Pillar", "Theatre Main",],
    "Library Main": ["Library Locked",],
    "Library Locked": [],
    "Keep Main": ["Keep Sunsetter", "Underbelly Hole", "Theatre Main",],
    "Keep Sunsetter": [],
    "Empty Bailey": ["Castle Main", "Tower Remains", "Theatre Pillar",],
    "Tower Remains": ["Underbelly Main", "The Great Door",],
    "Underbelly Main": ["Empty Bailey",],
    "Underbelly Hole": ["Underbelly Main",],
    "Theatre Main": ["Keep Main",],
    "Theatre Pillar": ["Theatre Main",],
    "The Great Door": [],
}
