from typing import NamedTuple, Callable, Dict, List, Optional
from BaseClasses import CollectionState
from .Rules import has_breaker, can_bounce, get_kicks, has_small_keys, navigate_darkrooms, can_slidejump


class RegionExit(NamedTuple):
    region: str
    access_rule: Callable[[CollectionState, int], bool] = lambda state, player: True
    breakable_wall: bool = False


region_table: Dict[str, List[RegionExit]] = {
    "Menu": [RegionExit("Dungeon Mirror")],
    "Dungeon Mirror": [RegionExit("Dungeon Strong Eyes",
                                  lambda state, player:
                                  state.has("Slide", player) and has_breaker(state, player)),
                       RegionExit("Underbelly Main", has_breaker),
                       RegionExit("Theatre Main",
                                  lambda state, player:
                                  any([
                                      get_kicks(state, player) >= 3 and has_breaker,
                                      state.has("Cling Gem", player) and has_breaker,
                                      can_slidejump(state, player) and get_kicks(state, player) >= 1 and has_breaker,
                                      can_bounce(state, player),
                                  ])
                                  )],
    "Dungeon Strong Eyes": [RegionExit("Castle Sansa", has_small_keys)],
    "Castle Sansa": [RegionExit("Library Main", has_breaker),
                     RegionExit("Keep Main"),
                     RegionExit("Empty Bailey"),
                     RegionExit("Theatre Pillar",
                                lambda state, player:
                                any([
                                    get_kicks(state, player) > 0,
                                    state.has("Cling Gem", player),
                                    state.has("Sunsetter", player),
                                ])),
                     RegionExit("Theatre Main",
                                lambda state, player:
                                any([
                                    state.has("Cling Gem", player),
                                    can_slidejump(state, player) and get_kicks(state, player) >= 4,
                                ]))],
    "Library Main": [RegionExit("Library Locked", has_small_keys)],
    "Library Locked": [],  # There's no point in connecting this back to library main.
    "Keep Main": [RegionExit("Keep Sunsetter",
                             lambda state, player:
                             any([
                                 has_small_keys(state, player),
                                 state.has("Cling Gem", player),
                                 get_kicks(state, player) >= 3,
                             ])),
                  RegionExit("Underbelly Hole",
                             lambda state, player:
                             any([
                                 get_kicks(state, player) > 0,
                                 state.has("Sunsetter", player),
                             ])),
                  RegionExit("Theatre Main",
                             lambda state, player:
                             any([
                                 state.has("Cling Gem", player) and get_kicks(state, player) >= 3,
                                 state.has("Cling Gem", player) and can_slidejump(state, player),
                             ]))],
    "Keep Sunsetter": [],
    "Empty Bailey": [RegionExit("Castle Sansa"),
                     RegionExit("Tower Remains",
                                lambda state, player:
                                any([
                                    get_kicks(state, player) > 0,
                                    state.has("Cling Gem", player),
                                    state.has_all(["Slide", "Sunsetter"], player)
                                ])),
                     RegionExit("Theatre Pillar")],
    "Tower Remains": [RegionExit("Underbelly Main",
                                 lambda state, player: state.has("Sunsetter", player)),
                      RegionExit("The Great Door",
                                 lambda state, player:
                                 state.has("Cling Gem", player) and get_kicks(state, player) >= 3)],
    "Underbelly Main": [RegionExit("Empty Bailey",
                                   lambda state, player:
                                   any([
                                       has_breaker(state, player),
                                       state.has("Sunsetter", player),
                                   ]))],
    "Underbelly Hole": [RegionExit("Underbelly Main",
                                   lambda state, player: state.has("Sunsetter", player))],
    "Theatre Main": [RegionExit("Keep Main",
                                lambda state, player: state.has("Cling Gem", player))],
    "Theatre Pillar": [RegionExit("Theatre Main",
                                  lambda state, player:
                                  any([
                                      state.has_all(["Sunsetter", "Cling Gem"], player),
                                      state.has("Sunsetter", player) and get_kicks(state, player) >= 4,
                                  ]))],
    "The Great Door": [],
}
