from dataclasses import dataclass
from Options import Toggle, Range, Choice, PerGameCommonOptions
from .constants.difficulties import NORMAL, HARD, EXPERT, LUNATIC


class LogicLevel(Choice):
    """
    The overall difficulty of the logic, used to determine the requirements to access locations and regions.

    Normal: Suitable for anyone who has beaten the game. Requires backflips and knowing where everything is.
    Hard: Can require some easier movement tricks such as cling climbing, and more nuanced movement like turning during an air kick.
    Expert: Can require more difficult movement tricks such as ultrahops and reverse wallkicks, and obscure knowledge.
    Lunatic: Can require extremely difficult jumps. No holds barred.
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


@dataclass
class PseudoregaliaOptions(PerGameCommonOptions):
    logic_level: LogicLevel
    obscure_tricks: ObscureTricks
