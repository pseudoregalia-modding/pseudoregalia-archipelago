from typing import NamedTuple, Callable, Dict, List, Optional
from BaseClasses import CollectionState
from Rules import has_breaker, can_bounce, get_kicks, has_small_keys, navigate_darkrooms


class RegionExit(NamedTuple):
    region: str
    access_rule: Callable[[CollectionState, int], bool] = lambda state, player: True
    breakable_wall: bool = False


region_table: Dict[str, List[RegionExit]] = {
    "Menu": [RegionExit("The Mirror")],
    "The Mirror": [RegionExit("Dungeon First Save Point", has_breaker)],
    "Dungeon First Save Point": [RegionExit("The Mirror", has_breaker),
                                 RegionExit("Dungeon By Slide", has_breaker)],
    "Dungeon By Slide": [RegionExit("Dungeon First Save Point", has_breaker),
                         RegionExit("Dungeon Near Strong Eyes",
                                    lambda state, player: state.has("Slide", player)),
                         RegionExit("Dungeon Escape Lower", has_breaker)],
    "Dungeon Near Strong Eyes": [RegionExit("Dungeon By Slide",
                                            lambda state, player: state.has("Slide", player)),
                                 RegionExit("Dungeon To Castle", has_small_keys)],
    "Dungeon To Castle": [RegionExit("The Mirror"),
                          RegionExit("Castle Halls")],
    "Dungeon Escape Lower": [RegionExit("Dungeon By Slide", has_breaker),
                             RegionExit("Dungeon Escape Upper",
                                        lambda state, player:
                                        any(
                                            can_bounce(player),
                                            state.has("Sunsetter", player) and get_kicks(state, player) >= 2,
                                            # state.has(state, "Cling Gem", player)
                                        )),
                             # underbelly exit
                             ],
    "Dungeon Escape Upper": [RegionExit("Dungeon Escape Lower"),
                             # theatre exit
                             ],

    "Castle Halls": [RegionExit("Dungeon To Castle"),
                     # library exit
                     # keep exit open
                     # keep exit to cyuiyce goat
                     # keep exit locked
                     # bailey exit
                     RegionExit("Castle To Theatre",
                                # rule for "player can reach Castle Through Bubble Rooms" ??
                                ),
                     RegionExit("Castle Through Bubble Rooms",
                                lambda state, player:
                                any(
                                    get_kicks(state, player) >= 3,
                                    get_kicks(state, player) >= 1 and state.has_all(
                                        ["Slide", "Solar Wind", "Sunsetter"], player)
                                ))],
    "Castle Through Bubble Rooms": [RegionExit("Castle Halls"),
                                    # theatre exit
                                    ],

}
