from BaseClasses import Location
from typing import NamedTuple, Dict


class PseudoregaliaLocationData(NamedTuple):
    region: str
    code: int = None


location_table = {
    "Dungeon - Dream Breaker": PseudoregaliaLocationData(
        # Dream Breaker can't really be shuffled right now but I would like to later
        code=2365810001,
        region="Dungeon Mirror"),
    "Dungeon - Slide": PseudoregaliaLocationData(
        code=2365810002,
        region="Dungeon Mirror"),
    "Castle - Indignation": PseudoregaliaLocationData(
        code=2365810003,
        region="Castle Sansa"),
    "Keep - Sunsetter": PseudoregaliaLocationData(
        code=2365810004,
        region="Keep Sunsetter"),
    "Keep - Strikebreak": PseudoregaliaLocationData(
        code=2365810005,
        region="Keep Main"),
    "Library - Sun Greaves": PseudoregaliaLocationData(
        code=2365810006,
        region="Library Main"),
    "Theatre - Soul Cutter": PseudoregaliaLocationData(
        code=2365810007,
        region="Theatre Main"),
    "Bailey - Solar Wind": PseudoregaliaLocationData(
        code=2365810008,
        region="Empty Bailey"),
    "Underbelly - Ascendant Light": PseudoregaliaLocationData(
        code=2365810009,
        region="Underbelly Main"),
    "Tower - Cling Gem": PseudoregaliaLocationData(
        code=2365810010,
        region="Tower Remains"),

    "Bailey - Major Key": PseudoregaliaLocationData(
        code=2365810011,
        region="Empty Bailey",),
    "Underbelly - Major Key": PseudoregaliaLocationData(
        code=2365810012,
        region="Underbelly Hole",),
    "Tower Remains - Major Key": PseudoregaliaLocationData(
        code=2365810013,
        region="Tower Remains",),
    "Sansa Keep - Major Key": PseudoregaliaLocationData(
        code=2365810014,
        region="Keep Main",),
    "Twilight Theatre - Major Key": PseudoregaliaLocationData(
        code=2365810015,
        region="Theatre Main",),
}
