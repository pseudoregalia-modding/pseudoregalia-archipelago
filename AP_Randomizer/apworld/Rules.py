from typing import Callable

from BaseClasses import CollectionState, MultiWorld


def get_encouragement_rule(multiworld: MultiWorld, player:int) -> Callable[[CollectionState], bool]:
    return lambda state: state.has("Dream Maker", player)