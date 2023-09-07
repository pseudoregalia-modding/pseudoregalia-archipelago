from BaseClasses import Location, MultiWorld
from typing import Callable, Dict, NamedTuple, Optional

from .constants import ZONE


class PseudoregaliaLocation(Location):
    game = "Pseudoregalia"


class PseudoregaliaLocationData(NamedTuple):
    region: str
    code: Optional[int] = None
    can_create: Callable[[MultiWorld, int], bool] = lambda multiworld, player: True
    locked_item: Optional[str] = None


location_data_table = {
    "Dungeon - Dream Breaker": PseudoregaliaLocationData(
        # Dream Breaker can't really be shuffled right now but I would like to later
        code=2365810001,
        region=ZONE.DUNGEON),
    "Dungeon - Slide": PseudoregaliaLocationData(
        code=2365810002,
        region=ZONE.DUNGEON),
    "Castle - Indignation": PseudoregaliaLocationData(
        code=2365810003,
        region=ZONE.CASTLE),
    "Keep - Sunsetter": PseudoregaliaLocationData(
        code=2365810004,
        region=ZONE.KEEP),
    "Keep - Strikebreak": PseudoregaliaLocationData(
        code=2365810005,
        region=ZONE.KEEP),
    "Library - Sun Greaves": PseudoregaliaLocationData(
        code=2365810006,
        region=ZONE.LIBRARY),
    "Theatre - Soul Cutter": PseudoregaliaLocationData(
        code=2365810007,
        region=ZONE.THEATRE),
    "Bailey - Solar Wind": PseudoregaliaLocationData(
        code=2365810008,
        region=ZONE.BAILEY),
    "Underbelly - Ascendant Light": PseudoregaliaLocationData(
        code=2365810009,
        region=ZONE.UNDERBELLY),
    "Tower - Cling Gem": PseudoregaliaLocationData(
        code=2365810010,
        region=ZONE.TOWER),
}

# locked_locations = {name: data for name, data in location_data_table.items() if data.locked_item}
