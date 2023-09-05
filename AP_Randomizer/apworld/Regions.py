from BaseClasses import MultiWorld, Region, Entrance
from typing import Dict, List, NamedTuple, Optional
from .Locations import PseudoregaliaLocation, location_data_table
from .constants import ZONE

# Code structure copied from Rogue Legacy's apworld.


class PseudoregaliaRegionData(NamedTuple):
    locations: Optional[List[str]]
    region_exits: Optional[List[str]]


def create_regions(multiworld: MultiWorld, player: int):
    region_table: Dict[str, PseudoregaliaRegionData] = {
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
        ZONE.CHAMBERS: PseudoregaliaRegionData(None, [ZONE.BAILEY])
    }

    # TODO: assign these dynamically by searching the location table
    region_table[ZONE.DUNGEON].locations.append("Dungeon - Dream Breaker")
    region_table[ZONE.DUNGEON].locations.append("Dungeon - Slide")
    region_table[ZONE.CASTLE].locations.append("Castle - Indignation")
    region_table[ZONE.LIBRARY].locations.append("Library - Sun Greaves")
    region_table[ZONE.KEEP].locations.append("Keep - Sunsetter")
    region_table[ZONE.KEEP].locations.append("Keep - Strikebreak")
    region_table[ZONE.THEATRE].locations.append("Theatre - Soul Cutter")
    region_table[ZONE.UNDERBELLY].locations.append("Underbelly - Ascendant Light")
    region_table[ZONE.BAILEY].locations.append("Bailey - Solar Wind")
    region_table[ZONE.TOWER].locations.append("Tower - Cling Gem")

    for region_name, region_data in region_table.items():
        region = Region(region_name, player, multiworld)
        if region_data.locations:
            for location_name in region_data.locations:
                location_data = location_data_table.get(location_name)
                location = PseudoregaliaLocation(
                    player, location_name, location_data.code, region)
                region.locations.append(location)

        if region_data.region_exits:
            for exit_name in region_data.region_exits:
                connection = Region(region_table[exit_name], player, multiworld)
                region.connect(connection)

        multiworld.regions.append(region)
