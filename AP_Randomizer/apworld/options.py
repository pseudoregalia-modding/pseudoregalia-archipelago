from dataclasses import dataclass
from Options import Toggle, Range, Choice, DefaultOnToggle, DeathLink
from .constants.difficulties import NORMAL, HARD, EXPERT, LUNATIC


class LogicLevel(Choice):
    """
    The overall difficulty of the logic, used to determine the requirements to access locations and regions.

    Normal: Suitable for anyone who has beaten the game. Requires backflips and knowing where everything is.
    Hard: Requires some easier movement tricks such as cling climbing, and more nuanced movement like turning during an air kick.
    Expert: Requires more difficult movement tricks such as ultrahops and reverse wallkicks, and obscure knowledge.
    Lunatic: Requires extremely difficult jumps and creative thinking. No holds barred. You have been warned.

    Currently only applies to Castle, Dungeon, and Library.
    """
    display_name = "Logic Level"
    option_normal = NORMAL
    option_hard = HARD
    option_expert = EXPERT
    option_lunatic = LUNATIC
    default = NORMAL


class ObscureTricks(Toggle):
    """
    Enables logic for tricks and paths that aren't difficult to execute but may not be obvious or commonly known.
    This option is forced on if logic level is set to Expert or Lunatic.

    Currently only applies to Castle, Dungeon, and Library.
    """
    display_name = "Obscure Tricks"


class SafeSmallKeys(DefaultOnToggle):
    """
    No locked doors are in logic until all small keys are obtainable.
    Prevents potential softlocks when spending small keys out of logic.

    Currently unused.
    """
    display_name = "Safe Small Keys"


class ProgressiveSlide(DefaultOnToggle):
    """
    Replaces Slide and Solar Wind with two Progressive Slide items.
    The first will give you Slide, and the second will give you Solar Wind.
    """
    display_name = "Progressive Slide"


class ProgressiveBreaker(DefaultOnToggle):
    """
    Replaces Dream Breaker, Strikebreak, and Soul Cutter with three Progressive Dream Breaker items.
    """
    display_name = "Progressive Dream Breaker"


class SplitSunGreaves(Toggle):
    """
    Replaces Sun Greaves and Heliacal Power with four individual Air Kicks.
    """
    display_name = "Split Sun Greaves"


pseudoregalia_options = {
    "logic_level": LogicLevel,
    "obscure_tricks": ObscureTricks,
    "progressive_slide": ProgressiveSlide,
    "progressive_breaker": ProgressiveBreaker,
    "split_sun_greaves": SplitSunGreaves,
    "death_link": DeathLink,
}
