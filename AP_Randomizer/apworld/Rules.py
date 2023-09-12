from BaseClasses import CollectionState, MultiWorld


def can_bounce(state: CollectionState, player: int) -> bool:
    return state.has_all(state, {"Dream Breaker", "Ascendant Light"}, player)


def get_kicks(state: CollectionState, player: int) -> int:
    kicks: int = 0
    if (state.has(state, "Sun Greaves", player)):
        kicks += 3
    kicks += state.count(state, "Sun Greaves", player)
    return kicks
