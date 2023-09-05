from BaseClasses import Location, MultiWorld
from typing import Callable, Dict, NamedTuple, Optional
from enum import Enum
from .Regions import ZONE


class PseudoregaliaLocation(Location):
    game = "Pseudoregalia"


class PseudoregaliaLocationData(NamedTuple):
    region: str
    address: Optional[int] = None
    can_create: Callable[[MultiWorld, int], bool] = lambda multiworld, player: True
    locked_item: Optional[str] = None


location_data_table = {
    ZONE.DUNGEON + " - Dream Breaker": PseudoregaliaLocationData(
        # Dream Breaker can't be shuffled right now but I would like to later
        address=2365810001,
        region=ZONE.DUNGEON),
    ZONE.DUNGEON + " - Slide": PseudoregaliaLocationData(
        address=2365810002,
        region=ZONE.DUNGEON),
    ZONE.CASTLE + " - Indignation": PseudoregaliaLocationData(
        address=2365810003,
        region=ZONE.CASTLE),
    ZONE.LIBRARY + " - Sun Greaves": PseudoregaliaLocationData(
        address=2365810004,
        region=ZONE.LIBRARY),
    ZONE.KEEP + " - Sunsetter": PseudoregaliaLocationData(
        address=2365810005,
        region=ZONE.KEEP),
    ZONE.KEEP + " - Strikebreak": PseudoregaliaLocationData(
        address=2365810006,
        region=ZONE.KEEP),
    ZONE.THEATRE + " - Soul Cutter": PseudoregaliaLocationData(
        address=2365810007,
        region=ZONE.THEATRE),
    ZONE.UNDERBELLY + " - Ascendant Light": PseudoregaliaLocationData(
        address=2365810008,
        region=ZONE.UNDERBELLY),
    ZONE.BAILEY + " - Solar Wind": PseudoregaliaLocationData(
        address=2365810009,
        region=ZONE.BAILEY),
    ZONE.TOWER + " - Cling Gem": PseudoregaliaLocationData(
        address=2365810010,
        region=ZONE.TOWER),
}

location_table = {name: data.address for name, data in location_data_table.items() if data.address is not None}
# locked_locations = {name: data for name, data in location_data_table.items() if data.locked_item}
77
