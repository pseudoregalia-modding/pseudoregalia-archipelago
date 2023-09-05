from BaseClasses import Location, MultiWorld
from typing import Callable, Dict, NamedTuple, Optional

class PseudoregaliaLocation(Location):
    game = "Pseudoregalia"

class PseudoregaliaLocationData(NamedTuple):
    region: str
    address: Optional[int] = None
    can_create: Callable[[MultiWorld, int], bool] = lambda multiworld, player: True
    locked_item: Optional[str] = None

location_data_table = {
    "The Beginning": PseudoregaliaLocationData(
        address=4206942067,
        region="The Dream"),
    "The Middle": PseudoregaliaLocationData(
        address=4206942068,
        region="The Dream"),
    "The End": PseudoregaliaLocationData(
        address=4206942069,
        region="The Dream",
        locked_item="Dream Maker"),
}

location_table = {name: data.address for name, data in location_data_table.items() if data.address is not None}
# locked_locations = {name: data for name, data in location_data_table.items() if data.locked_item}