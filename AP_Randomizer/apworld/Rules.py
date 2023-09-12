from BaseClasses import CollectionState, MultiWorld

# Placed for better legibility since dream breaker is an exclusive requirement for breakable walls


def has_breaker(state: CollectionState, player: int) -> bool:
    return state.has(state, "Dream breaker", player)


def can_bounce(state: CollectionState, player: int) -> bool:
    return state.has_all(state, {"Dream Breaker", "Ascendant Light"}, player)


def get_kicks(state: CollectionState, player: int) -> int:
    kicks: int = 0
    if (state.has(state, "Sun Greaves", player)):
        kicks += 3
    kicks += state.count(state, "Sun Greaves", player)
    return kicks
