
from enum import Enum

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
