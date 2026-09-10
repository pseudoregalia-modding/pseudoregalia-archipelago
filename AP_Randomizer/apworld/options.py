from dataclasses import dataclass

from Options import Choice, DefaultOnToggle, PerGameCommonOptions, Toggle

from .constants.difficulties import EXPERT, HARD, LUNATIC, NORMAL
from .logic import player_start_enum, pseudoregalia_data


class LogicLevel(Choice):
    """
    The overall difficulty of the logic, used to determine the requirements to access locations and regions.

    Normal: Suitable for anyone who has beaten the game. Requires backflips and knowing where everything is.
    Hard: Requires some easier movement tricks such as cling climbing and backwards ultras using solar wind,
          and more nuanced movement like turning during an air kick.
    Expert: Requires more difficult movement tricks such as ultrahops and reverse wallkicks, and obscure knowledge.
    Lunatic: Requires extremely difficult jumps and creative thinking. No holds barred. You have been warned.
    """
    display_name = "Logic Level"
    option_normal = NORMAL
    option_hard = HARD
    option_expert = EXPERT
    option_lunatic = LUNATIC
    default = NORMAL


class ObscureLogic(Toggle):
    """
    Enables logic for obscure knowledge and creative pathing that isn't difficult to execute
    but may not be obvious or commonly known.
    This option is forced on if logic level is set to Expert or Lunatic.
    """
    display_name = "Obscure Logic"


SpawnPoint = type("SpawnPoint", (Choice,), {
    "__module__": __name__,
    "display_name": "Spawn Point",
    "__doc__": "Determines where you will spawn into the game when creating a new file.\n\n"
               "Some spawns have special behaviors if they are selected or randomly chosen which can affect starting "
               "inventory and\nlogic. Refer to the game page or the readme in the github repo for more information.",
    **{f"option_{data.name}": player_start_enum[data.player_start] for data in pseudoregalia_data.spawn_points},
    "default": next(player_start_enum[data.player_start] for data in pseudoregalia_data.spawn_points if data.default),
})


class SafeSmallKeys(DefaultOnToggle):
    """
    No locked doors are in logic until all small keys are obtainable.
    Prevents potential softlocks when spending small keys out of logic.

    Currently unused.
    """
    display_name = "Safe Small Keys"


class ProgressiveBreaker(DefaultOnToggle):
    """
    Replaces Dream Breaker, Strikebreak, and Soul Cutter with three Progressive Dream Breaker items.
    """
    display_name = "Progressive Dream Breaker"


class ProgressiveSlide(DefaultOnToggle):
    """
    Replaces Slide and Solar Wind with two Progressive Slide items.
    """
    display_name = "Progressive Slide"


class SplitSunGreaves(Toggle):
    """
    Replaces Sun Greaves and Heliacal Power with four individual Air Kicks.
    """
    display_name = "Split Sun Greaves"


class SplitClingGem(Toggle):
    """
    Replaces Cling Gem with three Cling Shard items.
    Each Cling Shard increases your wall run limit by two.
    """
    display_name = "Split Cling Gem"


class UltraCap(Choice):
    """
    Determines how the ultra height cap behaves. This only matters for Expert+ logic.

    Vanilla: Ultras have the lower blue cap unless Solar Wind has been obtained and is active.
    Full Gold: Ultras always have the higher gold cap.
    """
    display_name = "Ultra Cap"
    option_vanilla = 0
    option_full_gold = 1
    default = 0


class StartWithBreaker(Toggle):
    """
    Places Dream Breaker (or one Progressive Dream Breaker) in the starting inventory.
    """
    display_name = "Start With Breaker"


class StartWithMap(Toggle):
    """
    Places the map item (Memento) in the starting inventory.
    """
    display_name = "Start With Map"


class RandomizeTimeTrials(Toggle):
    """
    Opens the time trials for randomization and puts the outfits in the item pool.
    If turned off, the time trials will not have items and the outfits are placed in the starting inventory.
    """
    display_name = "Randomize Time Trials"


class RandomizeGoats(Toggle):
    """
    Adds goatlings as locations. Talk to the goatling to get the item.
    This option adds 19 locations.
    For each location added, an essentially useless filler item is also added to the item pool.
    """
    display_name = "Randomize Goats"


class RandomizeChairs(Toggle):
    """
    Adds places Sybil can sit as locations. Sit down to get the item.
    This option adds 16 locations.
    For each location added, an essentially useless filler item is also added to the item pool.
    """
    display_name = "Randomize Chairs"


class RandomizeBooks(Toggle):
    """
    Adds books in the library as locations. Read the book to get the item.
    This option adds 13 locations.
    For each location added, an essentially useless filler item is also added to the item pool.
    """
    display_name = "Randomize Books"


class RandomizeNotes(Toggle):
    """
    Adds notes as locations. Read the note to get the item.
    This option adds 4 locations.
    For each location added, an essentially useless filler item is also added to the item pool.
    """
    display_name = "Randomize Notes"


class MajorKeyHints(DefaultOnToggle):
    """
    Changes the text on the tombstones near the Great Door to tell you the locations of your Major Keys.
    Also makes it so reading the tombstones will create hints.
    """
    display_name = "Major Key Hints"


@dataclass
class PseudoregaliaOptions(PerGameCommonOptions):
    logic_level: LogicLevel
    obscure_logic: ObscureLogic
    spawn_point: SpawnPoint
    ultra_cap: UltraCap
    progressive_breaker: ProgressiveBreaker
    progressive_slide: ProgressiveSlide
    split_sun_greaves: SplitSunGreaves
    split_cling_gem: SplitClingGem
    start_with_breaker: StartWithBreaker
    start_with_map: StartWithMap
    randomize_time_trials: RandomizeTimeTrials
    randomize_goats: RandomizeGoats
    randomize_chairs: RandomizeChairs
    randomize_books: RandomizeBooks
    randomize_notes: RandomizeNotes
    major_key_hints: MajorKeyHints
