
from __future__ import annotations

from typing import Optional, Dict, Tuple, List

from simple_types import Player

def _make_column_ternary_to_column_encoded(n, q) -> Dict[int, int]:

    to_be_processed: List[Tuple[Player, ...]] = [tuple()]
    columns = []
    while to_be_processed:
        tup = to_be_processed.pop()

        ternary = 0
        for digit in reversed(tup):
            ternary *= 3
            if digit == Player.A:
                ternary += 1
            elif digit == Player.B:
                ternary += 2

        columns.append(ternary)

        if len(tup) < n:
            # check if the last q entries are winning.
            if len(tup) >= q:
                if len(set(tup[-q:])) == 1:
                    continue

            # We can still add a chip.
            to_be_processed.append(tup + (Player.A, ))
            to_be_processed.append(tup + (Player.B, ))

    columns.sort()

    return dict((ternary, encoded) for (encoded, ternary) in enumerate(columns))


class Board:

    H_SIZE = 6
    V_SIZE = 5
    Q = 4

    def __init__(self, entries):
        entries_tuple = tuple(entry for entry in entries)
        if len(entries_tuple) != Board.H_SIZE * Board.V_SIZE:
            raise RuntimeError("Bad number of entries.")

        self.entries = entries_tuple
        self.column_ternary_to_column_encoded = _make_column_ternary_to_column_encoded(Board.V_SIZE, Board.Q)

    def mover(self) -> Player:
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
        raise RuntimeError("Bad mover")

    @staticmethod
    def is_valid_coordinate(x: int, y: int) -> bool:
        return (0 <= x < Board.H_SIZE) and (0 <= y < Board.V_SIZE)

    def get(self, x: int, y: int) -> Player:
        if not Board.is_valid_coordinate(x, y):
            raise RuntimeError("Bad get")
        return self.entries[y * Board.H_SIZE + x]

    def winner(self) -> Player:
        player_a_wins = False
        player_b_wins = False

        for y in range(Board.V_SIZE):
            for x in range(Board.H_SIZE):

                player = self.entries[y * Board.H_SIZE + x]

                if player != Player.NONE:

                    directions = ((0, 1), (1, 0), (1, 1), (1, -1))

                    for (dx, dy) in directions:

                        if Board.is_valid_coordinate(x + dx * (Board.Q - 1), y + dy * (Board.Q - 1)):

                            found_win = True

                            for i in range(1, Board.Q):

                                if self.get(x + dx * i, y + dy * i) != player:
                                    found_win = False

                            if found_win:
                                if player == Player.A:
                                    player_a_wins = True
                                elif player == Player.B:
                                    player_b_wins = True

        if player_a_wins and player_b_wins:
            raise RuntimeError("Multiple winners")

        if player_a_wins:
            return Player.A

        if player_b_wins:
            return Player.B

        return Player.NONE

    def print(self) -> None:
        for y in reversed(range(Board.V_SIZE)):
            for x in range(Board.H_SIZE):
                print(self.entries[y * Board.H_SIZE + x], end='')
            print()
        print()

    def encode(self) -> int:
        encoding = 0
        for x in range(Board.H_SIZE):
            ternary = 0
            for y in reversed(range(Board.V_SIZE)):
                ternary *= 3
                player = self.get(x, y)
                if player == Player.A:
                    ternary += 1
                elif player == Player.B:
                    ternary += 2
            encoding *= len(self.column_ternary_to_column_encoded)
            encoding += self.column_ternary_to_column_encoded[ternary]

        return encoding

    def normalize(self) -> Board:

        entries = []
        for y in range(Board.V_SIZE):
            for x in range(Board.H_SIZE):
                x_mirrored = (Board.H_SIZE -1) - x
                entries.append(self.get(x_mirrored, y))

        horizontal_mirror = Board(entries)

        return min(self, horizontal_mirror)

    def __lt__(self, rhs) -> bool:
        return self.encode() < rhs.encode()

    def drop(self, col) -> Optional[Board]:

        winner = self.winner()
        if winner != Player.NONE:
            return None

        for y in range(Board.V_SIZE):
            if self.get(col, y) == Player.NONE:
                entries = list(self.entries)
                entries[y * Board.H_SIZE + col] = self.mover()
                drop_board = Board(entries)
                return drop_board

        return None
