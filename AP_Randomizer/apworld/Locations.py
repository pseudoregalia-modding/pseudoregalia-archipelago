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

    "Underbelly - Heliacal Power": PseudoregaliaLocationData(
        code=2365810011,
        region="Underbelly Main",),
    "Theatre - Aerial Finesse": PseudoregaliaLocationData(
        code=2365810012,
        region="Theatre Pillar",),
    "Castle - Pilgrimage": PseudoregaliaLocationData(
        code=2365810013,
        region="Castle Sansa",),
    "Bailey - Empathy": PseudoregaliaLocationData(
        code=2365810014,
        region="Empty Bailey",),
    "Theatre - Empathy": PseudoregaliaLocationData(
        code=2365810015,
        region="Theatre Main",),
    "Dungeon - Good Graces": PseudoregaliaLocationData(
        code=2365810016,
        region="Dungeon Mirror",),
    "Castle - Good Graces": PseudoregaliaLocationData(
        code=2365810017,
        region="Castle Sansa",),
    "Underbelly - Martial Prowess": PseudoregaliaLocationData(
        code=2365810018,
        region="Underbelly Main",),
    "Castle - Clear Mind": PseudoregaliaLocationData(
        code=2365810019,
        region="Castle Sansa",),
    "Library - Clear Mind": PseudoregaliaLocationData(
        code=2365810020,
        region="Library Locked",),
    "Keep - Clear Mind": PseudoregaliaLocationData(
        code=2365810021,
        region="Keep Main",),
    "Castle - The Professional": PseudoregaliaLocationData(
        code=2365810022,
        region="Castle Sansa",),

    "Bailey - Major Key": PseudoregaliaLocationData(
        code=2365810046,
        region="Empty Bailey",),
    "Underbelly - Major Key": PseudoregaliaLocationData(
        code=2365810047,
        region="Underbelly Hole",),
    "Tower - Major Key": PseudoregaliaLocationData(
        code=2365810048,
        region="Tower Remains",),
    "Keep - Major Key": PseudoregaliaLocationData(
        code=2365810049,
        region="Keep Main",),
    "Theatre - Major Key": PseudoregaliaLocationData(
        code=2365810050,
        region="Theatre Main",),
}
