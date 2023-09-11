from typing import NamedTuple, Callable, Dict, List
from BaseClasses import CollectionState


class RegionExit(NamedTuple):
    region: str
    access_rule: Callable[[CollectionState, int], bool] = lambda state, player: True


region_table: Dict[str, List[RegionExit]] = {
    "Menu": [RegionExit("The Mirror")],
}
