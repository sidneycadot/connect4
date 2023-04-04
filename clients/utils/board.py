"""Represent a game board with some basic functionality."""

from typing import Sequence, Optional

from .simple_types import Player, Outcome


class Board:
    """Represents a connect-4 board."""

    def __init__(self, info, entries: Sequence[Player]):

        entries_tuple = tuple(entry for entry in entries)
        if len(entries_tuple) != info.h_size * info.v_size:
            raise RuntimeError("Bad number of entries.")

        self.info = info
        self.entries = entries_tuple

    def is_valid_coordinate(self, x: int, y: int) -> bool:
        info = self.info
        return (0 <= x < info.h_size) and (0 <= y < info.v_size)

    def get(self, x: int, y: int) -> Player:
        if not self.is_valid_coordinate(x, y):
            raise RuntimeError("Bad get.")
        info = self.info
        return self.entries[y * info.h_size + x]

    def mover(self) -> Optional[Player]:
        """Determine which player has the move."""

        if self.outcome() != Outcome.INDETERMINATE:
            return None

        a_min_b = 0
        for entry in self.entries:
            if entry == Player.A:
                a_min_b += 1
            elif entry == Player.B:
                a_min_b -= 1
        if a_min_b == 0:
            return Player.A
        if a_min_b == 1:
            return Player.B
        raise RuntimeError("Bad mover.")

    def outcome(self) -> Outcome:
        """Check if the Board has a definite outcome."""

        info = self.info

        directions = ((0, 1), (1, 0), (1, 1), (1, -1))

        player_a_wins = False
        player_b_wins = False

        for y in range(info.v_size):
            for x in range(info.h_size):
                player = self.get(x, y)
                if player != Player.NONE:
                    for (dx, dy) in directions:
                        # Check that there is enough space for a line of 'q' elements.
                        if self.is_valid_coordinate(x + dx * (info.connect_q - 1), y + dy * (info.connect_q - 1)):
                            # Check that all (q - 1) elements are of player 'player'.
                            if all(self.get(x + dx * i, y + dy * i) == player for i in range(1, info.connect_q)):
                                if player == Player.A:
                                    player_a_wins = True
                                elif player == Player.B:
                                    player_b_wins = True

        if player_a_wins and player_b_wins:
            raise RuntimeError("Multiple winners found (board invalid).")

        if player_a_wins:
            return Outcome.A_WINS

        if player_b_wins:
            return Outcome.B_WINS

        # We will return "NONE" if the game is a draw (no move possible), or INDETERMINATE
        # if moves are still possible.

        for x in range(info.h_size):
            if self.get(x, info.v_size - 1) == Player.NONE:
                # A move is available in column 'x'.
                return Outcome.INDETERMINATE

        return Outcome.DRAW

    def print(self) -> None:
        info = self.info
        player_map = {
            Player.A : 'A',
            Player.B : 'B',
            Player.NONE : '.'
        }
        for y in reversed(range(info.v_size)):
            for x in range(info.h_size):
                c = player_map[self.get(x, y)]
                print(c, end='')
            print()
        print()

    def execute_move(self, col: int) -> Optional['Board']:
        """Try to drop a chip of the player having the move in column 'col'."""

        outcome = self.outcome()
        if outcome != Outcome.INDETERMINATE:
            # The game has finished, no more moves are possible.
            return None

        info = self.info

        for y in range(info.v_size):
            if self.get(col, y) == Player.NONE:
                entries = list(self.entries)
                entries[y * info.h_size + col] = self.mover()
                changed_board = Board(info, entries)
                return changed_board

        return None
