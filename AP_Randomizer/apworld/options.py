from dataclasses import dataclass
from Options import Toggle, Range, Choice, PerGameCommonOptions


class LogicLevel(Choice):
    """
    The overall difficulty of the logic, used to determine the requirements to access locations and regions.

    Normal: Suitable for anyone who has beaten the game. Requires backflips and knowing where everything is.
    Hard: Can require some easier movement tricks such as cling climbing, and more nuanced movement like turning during an air kick.
    Expert: Can require more difficult movement tricks such as ultrahops and reverse wallkicks, and obscure knowledge.
    Lunatic: Can require extremely difficult jumps. No holds barred.
    """
    display_name = "Logic Level"
    option_normal = 0
    option_hard = 1
    option_expert = 2
    option_lunatic = 3
    default = 0


class ObscureTricks(Toggle):
    display_name = "Obscure Tricks"


@dataclass
class PseudoregaliaOptions(PerGameCommonOptions):
    logic_level: LogicLevel
    obscure_tricks: ObscureTricks
