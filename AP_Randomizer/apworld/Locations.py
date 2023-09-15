from BaseClasses import Location
from typing import NamedTuple, Dict


class PseudoregaliaLocation(Location):
    game: str = "Pseudoregalia"


class PseudoregaliaLocationData(NamedTuple):
    region: str
    code: int = None


location_table = {
    "Dungeon - Dream Breaker": PseudoregaliaLocationData(
        # Dream Breaker can't really be shuffled right now but I would like to later
        code=2365810001,
        region="The Mirror"),
    "Dungeon - Slide": PseudoregaliaLocationData(
        code=2365810002,
        region="Dungeon By Slide"),
    "Castle - Indignation": PseudoregaliaLocationData(
        code=2365810003,
        region="Castle Halls"),
    "Keep - Sunsetter": PseudoregaliaLocationData(
        code=2365810004,),
    "Keep - Strikebreak": PseudoregaliaLocationData(
        code=2365810005,),
    "Library - Sun Greaves": PseudoregaliaLocationData(
        code=2365810006,),
    "Theatre - Soul Cutter": PseudoregaliaLocationData(
        code=2365810007,),
    "Bailey - Solar Wind": PseudoregaliaLocationData(
        code=2365810008,),
    "Underbelly - Ascendant Light": PseudoregaliaLocationData(
        code=2365810009,),
    "Tower - Cling Gem": PseudoregaliaLocationData(
        code=2365810010,),
}
