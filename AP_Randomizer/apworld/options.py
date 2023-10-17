from dataclasses import dataclass
from Options import Toggle, Range, Choice, PerGameCommonOptions, DefaultOnToggle
from .constants.difficulties import NORMAL, HARD, EXPERT, LUNATIC


class LogicLevel(Choice):
    """
    The overall difficulty of the logic, used to determine the requirements to access locations and regions.

    Normal: Suitable for anyone who has beaten the game. Requires backflips and knowing where everything is.
    Hard: Requires some easier movement tricks such as cling climbing, and more nuanced movement like turning during an air kick.
    Expert: Requires more difficult movement tricks such as ultrahops and reverse wallkicks, and obscure knowledge.
    Lunatic: Requires extremely difficult jumps and creative thinking. No holds barred. You have been warned.

    Currently only applies to Castle Sansa.
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
    """
    display_name = "Obscure Tricks"


class SafeSmallKeys(DefaultOnToggle):
    """
    No locked doors are in logic until all small keys are obtainable.
    Prevents potential softlocks when spending small keys out of logic.

    Currently unused.
    """
    display_name = "Safe Small Keys"


@dataclass
class PseudoregaliaOptions(PerGameCommonOptions):
    logic_level: LogicLevel
    obscure_tricks: ObscureTricks
    safe_small_keys: SafeSmallKeys
