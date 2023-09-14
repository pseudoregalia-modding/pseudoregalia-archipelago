from typing import NamedTuple, Callable, Dict, List, Optional
from BaseClasses import CollectionState
from Rules import has_breaker, can_bounce, get_kicks, has_small_keys, navigate_darkrooms


class PseudoregaliaRegionData(NamedTuple):
    locations: Optional[List[str]]
    region_exits: Optional[List[str]]


class RegionExit(NamedTuple):
    region: str
    access_rule: Callable[[CollectionState, int], bool] = lambda state, player: True


region_table: Dict[str, List[RegionExit]] = {
    "Menu": [RegionExit("The Mirror")],
    "The Mirror": [RegionExit("Dungeon: First Save Point", has_breaker)],
    "Dungeon First Save Point": [RegionExit("The Mirror", has_breaker),
                                 RegionExit("Dungeon By Slide", has_breaker)],
    "Dungeon By Slide": [RegionExit("Dungeon First Save Point", has_breaker),
                         RegionExit("Dungeon Near Strong Eyes",
                                    lambda state, player: state.has(state, "Slide", player)),
                         RegionExit("Dungeon Escape Lower", has_breaker)],
    "Dungeon Near Strong Eyes": [RegionExit("Dungeon Near Slide",
                                            lambda state, player: state.has(state, "Slide", player)),
                                 RegionExit("Dungeon By Castle", has_small_keys)],
    "Dungeon By Castle": [RegionExit("The Mirror"),
                          RegionExit("Castle Halls")],
    "Dungeon Escape Lower": [RegionExit("Dungeon By Slide", has_breaker),
                             RegionExit("Dungeon Escape Upper",
                                        lambda state, player:
                                        any(
                                            can_bounce(state, player),
                                            state.has(state, "Sunsetter", player) and get_kicks == 2,
                                            # state.has(state, "Cling Gem", player)
                                        ))],
    "Dungeon Escape Upper": [RegionExit("Dungeon Escape Lower")],
}
