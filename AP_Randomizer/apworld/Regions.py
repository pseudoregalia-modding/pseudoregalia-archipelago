from BaseClasses import MultiWorld, Region, Entrance
from typing import Dict, List, NamedTuple, Optional
from enum import Enum
from .Locations import PseudoregaliaLocationData, location_table

# Code structure copied from Rogue Legacy's apworld.


class ZONE(Enum):
    DUNGEON = "Dungeon"
    CASTLE = "Castle"
    KEEP = "Keep"
    LIBRARY = "Library"
    THEATRE = "Theatre"
    BAILEY = "Bailey"
    UNDERBELLY = "Underbelly"
    TOWER = "Tower"
    CHAMBERS = "Chambers"


class PseudoregaliaRegionData(NamedTuple):
    locations: Optional[List[str]]
    region_exits: Optional[List[str]]


def create_regions(multiworld: MultiWorld, player: int):
    regions: Dict[str, PseudoregaliaRegionData] = {
        "Menu": PseudoregaliaRegionData(None, [ZONE.DUNGEON]),
        ZONE.DUNGEON: PseudoregaliaRegionData([], [ZONE.CASTLE, ZONE.UNDERBELLY, ZONE.THEATRE]),
        ZONE.CASTLE: PseudoregaliaRegionData([], [ZONE.DUNGEON, ZONE.KEEP, ZONE.BAILEY, ZONE.THEATRE, ZONE.LIBRARY]),
        ZONE.KEEP: PseudoregaliaRegionData([], [ZONE.CASTLE, ZONE.THEATRE, ZONE.UNDERBELLY]),
        ZONE.LIBRARY: PseudoregaliaRegionData([], [ZONE.CASTLE]),
        ZONE.THEATRE: PseudoregaliaRegionData([], [ZONE.CASTLE, ZONE.DUNGEON, ZONE.BAILEY, ZONE.KEEP]),
        ZONE.BAILEY: PseudoregaliaRegionData([], [ZONE.CASTLE, ZONE.UNDERBELLY, ZONE.TOWER, ZONE.THEATRE]),
        ZONE.UNDERBELLY: PseudoregaliaRegionData([], [ZONE.DUNGEON, ZONE.KEEP, ZONE.BAILEY]),
        ZONE.TOWER: PseudoregaliaRegionData([], [ZONE.BAILEY, ZONE.CHAMBERS]),
        # Chambers here for completeness but may be removed later as it's pretty obsolete
        ZONE.CHAMBERS: PseudoregaliaRegionData(None, ZONE.BAILEY)
    }
    multiworld.regions.append(create_region())


def create_region(multiworld: MultiWorld, player: int, region_name: str, region_data: PseudoregaliaRegionData):
    region = Region(region_name, player, multiworld)
    if region_data.locations:
        for location_name in region_data.locations:
            location_data = location_table.get(location_name)
            location = PseudoregaliaLocationData(
                player, location_name, location_data.code if location_data else None, region)
            region.locations.append(location)

    if region_data.region_exits:
        for exit in region_data.region_exits:
            entrance = Entrance(player, exit, region)
            region.exits.append(entrance)

    return region
