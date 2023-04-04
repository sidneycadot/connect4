"""Simple types for connect-4."""

from enum import Enum
from typing import NamedTuple


class Player(Enum):
    """Represents a player."""
    NONE = 0
    A = 1
    B = 2


class Outcome(Enum):
    """Represents the outcome of a game."""
    A_WINS = 0
    B_WINS = 1
    DRAW = 2
    INDETERMINATE = 3


class Score(NamedTuple):
    """Represents the score of a position, consisting of an Outcome and the number of plies before it is definitive."""
    outcome: Outcome
    ply    : int
