from BaseClasses import Location
from typing import NamedTuple, Dict, Optional


class PseudoregaliaLocationData(NamedTuple):
    region: str
    code: int = None
    locked_item: Optional[str] = None
    show_in_spoiler: bool = True


location_table = {
    "Dungeon - Dream Breaker": PseudoregaliaLocationData(
        # Dream Breaker can't really be shuffled right now but I would like to later
        code=2365810001,
        region="Dungeon Mirror",
        locked_item="Dream Breaker"),
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

    "Underbelly - Locked Door": PseudoregaliaLocationData(
        code=2365810011,
        region="Underbelly Main",),
    "Theatre - Corner Beam": PseudoregaliaLocationData(
        code=2365810012,
        region="Theatre Pillar",),
    "Castle - Floater In Courtyard": PseudoregaliaLocationData(
        code=2365810013,
        region="Castle Sansa",),
    "Bailey - Cheese Bell": PseudoregaliaLocationData(
        code=2365810014,
        region="Empty Bailey",),
    "Theatre - Locked Door": PseudoregaliaLocationData(
        code=2365810015,
        region="Theatre Main",),
    "Dungeon - Dark Orbs": PseudoregaliaLocationData(
        code=2365810016,
        region="Dungeon Mirror",),
    "Castle - Near Theatre Front": PseudoregaliaLocationData(
        code=2365810017,
        region="Castle Sansa",),
    "Underbelly - Strikebreak Wall": PseudoregaliaLocationData(
        code=2365810018,
        region="Underbelly Main",),
    "Castle - High Climb From Courtyard": PseudoregaliaLocationData(
        code=2365810019,
        region="Castle Sansa",),
    "Library - Locked Door Across": PseudoregaliaLocationData(
        code=2365810020,
        region="Library Locked",),
    "Keep - Near Theatre": PseudoregaliaLocationData(
        code=2365810021,
        region="Keep Main",),
    "Castle - Locked Door": PseudoregaliaLocationData(
        code=2365810022,
        region="Castle Sansa",),

    "Dungeon - Rafters": PseudoregaliaLocationData(
        code=2365810023,
        region="Dungeon Strong Eyes",),
    "Dungeon - Strong Eyes": PseudoregaliaLocationData(
        code=2365810024,
        region="Dungeon Strong Eyes",),
    "Castle - Platform In Main Halls": PseudoregaliaLocationData(
        code=2365810025,
        region="Castle Sansa",),
    "Castle - Tall Room Near Wheel Crawlers": PseudoregaliaLocationData(
        code=2365810026,
        region="Castle Sansa",),
    "Keep - Levers Room": PseudoregaliaLocationData(
        code=2365810027,
        region="Keep Main",),
    "Bailey - Inside Building": PseudoregaliaLocationData(
        code=2365810028,
        region="Empty Bailey",),
    "Underbelly - Main Room": PseudoregaliaLocationData(
        code=2365810029,
        region="Underbelly Main",),

    "Dungeon - Alcove Near Mirror": PseudoregaliaLocationData(
        code=2365810030,
        region="Dungeon Mirror",),
    "Dungeon - Past Poles": PseudoregaliaLocationData(
        code=2365810031,
        region="Dungeon Strong Eyes",),
    "Castle - Alcove Near Dungeon": PseudoregaliaLocationData(
        code=2365810032,
        region="Castle Sansa",),
    "Castle - Corner Corridor": PseudoregaliaLocationData(
        code=2365810033,
        region="Castle Sansa",),
    "Castle - Wheel Crawlers": PseudoregaliaLocationData(
        code=2365810034,
        region="Castle Sansa",),
    "Castle - Alcove Near Scythe Corridor": PseudoregaliaLocationData(
        code=2365810035,
        region="Castle Sansa",),
    "Castle - Balcony": PseudoregaliaLocationData(
        code=2365810036,
        region="Castle Sansa",),
    "Library - Upper Back": PseudoregaliaLocationData(
        code=2365810037,
        region="Library Main",),
    "Library - Locked Door Left": PseudoregaliaLocationData(
        code=2365810038,
        region="Library Locked",),
    "Keep - Alcove Near Locked Door": PseudoregaliaLocationData(
        code=2365810039,
        region="Keep Main",),
    "Bailey - Center Steeple": PseudoregaliaLocationData(
        code=2365810040,
        region="Empty Bailey",),
    "Underbelly - Rafters Near Keep": PseudoregaliaLocationData(
        code=2365810041,
        region="Underbelly Hole",),
    "Underbelly - Building Near Little Guy": PseudoregaliaLocationData(
        code=2365810042,
        region="Underbelly Main",),
    "Underbelly - Alcove Near Light": PseudoregaliaLocationData(
        code=2365810043,
        region="Underbelly Main",),
    "Theatre - Murderous Goat": PseudoregaliaLocationData(
        code=2365810044,
        region="Theatre Main",),
    "Theatre - Back Of Auditorium": PseudoregaliaLocationData(
        code=2365810045,
        region="Theatre Main",),

    "Bailey - Major Key": PseudoregaliaLocationData(
        code=2365810046,
        region="Empty Bailey",),
    "Underbelly - Major Key": PseudoregaliaLocationData(
        code=2365810047,
        region="Underbelly Hole",),
    "Tower - Major Key": PseudoregaliaLocationData(
        code=2365810048,
        region="The Great Door",),
    "Keep - Major Key": PseudoregaliaLocationData(
        code=2365810049,
        region="Keep Main",),
    "Theatre - Major Key": PseudoregaliaLocationData(
        code=2365810050,
        region="Theatre Main",),

    "Dungeon - Unlock Door": PseudoregaliaLocationData(
        region="Dungeon Strong Eyes",
        locked_item="Unlocked Door",
        show_in_spoiler=False),
    "Castle - Unlock Door (Professionalism)": PseudoregaliaLocationData(
        region="Castle Sansa",
        locked_item="Unlocked Door",
        show_in_spoiler=False),
    "Castle - Unlock Door (Sansa Keep)": PseudoregaliaLocationData(
        region="Castle Sansa",
        locked_item="Unlocked Door",
        show_in_spoiler=False),
    "Keep - Unlock Door": PseudoregaliaLocationData(
        region="Keep Main",
        locked_item="Unlocked Door",
        show_in_spoiler=False),
    "Library - Unlock Door": PseudoregaliaLocationData(
        region="Library Main",
        locked_item="Unlocked Door",
        show_in_spoiler=False),
    "Theatre - Unlock Door": PseudoregaliaLocationData(
        region="Theatre Main",
        locked_item="Unlocked Door",
        show_in_spoiler=False),
    "Underbelly - Unlock Door": PseudoregaliaLocationData(
        region="Underbelly Main",
        locked_item="Unlocked Door",
        show_in_spoiler=False),

    "D S T RT ED M M O   Y": PseudoregaliaLocationData(
        region="The Great Door",
        locked_item="Something Worth Being Awake For"),
}
