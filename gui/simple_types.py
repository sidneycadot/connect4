"""Simple support types."""

from enum import Enum
from typing import NamedTuple

class Player(Enum):
    NONE = 0
    A = 1
    B = 2

    def __str__(self):
        if self == Player.NONE:
            return '.'
        if self == Player.A:
            return 'A'
        if self == Player.B:
            return 'B'
        raise ValueError("Bad Player value.")


class Outcome(Enum):
    A_WINS = 0
    B_WINS = 1
    DRAW = 2
    INDETERMINATE = 3



class Score(NamedTuple):
    outcome: Outcome
    ply    : int
