
from __future__ import annotations

from simple_types import Player


class Board:

    def __init__(self, info, entries):

        entries_tuple = tuple(entry for entry in entries)
        if len(entries_tuple) != info.h_size * info.v_size:
            raise RuntimeError("Bad number of entries.")

        self.info = info
        self.entries = entries_tuple

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

    def is_valid_coordinate(self, x: int, y: int) -> bool:
        info = self.info
        return (0 <= x < info.h_size) and (0 <= y < info.v_size)

    def get(self, x: int, y: int) -> Player:
        if not self.is_valid_coordinate(x, y):
            raise RuntimeError("Bad get")
        info = self.info
        return self.entries[y * info.h_size + x]

    def winner(self) -> Player:

        info = self.info

        player_a_wins = False
        player_b_wins = False

        for y in range(info.v_size):
            for x in range(info.h_size):

                player = self.entries[y * info.h_size + x]

                if player != Player.NONE:

                    directions = ((0, 1), (1, 0), (1, 1), (1, -1))

                    for (dx, dy) in directions:

                        if self.is_valid_coordinate(x + dx * (info.connect_q - 1), y + dy * (info.connect_q - 1)):

                            found_win = True

                            for i in range(1, info.connect_q):

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
        for y in reversed(range(self.v_size)):
            for x in range(self.h_size):
                print(self.entries[y * self.h_size + x], end='')
            print()
        print()

    def encode(self) -> int:
        info = self.info
        encoding = 0
        for x in range(info.h_size):
            ternary = 0
            for y in reversed(range(info.v_size)):
                ternary *= 3
                player = self.get(x, y)
                if player == Player.A:
                    ternary += 1
                elif player == Player.B:
                    ternary += 2
            encoding *= len(info.column_ternary_to_column_encoded)
            encoding += info.column_ternary_to_column_encoded[ternary]

        return encoding

    def normalize(self) -> Board:

        info = self.info

        entries = []
        for y in range(info.v_size):
            for x in range(info.h_size):
                x_mirrored = (info.h_size -1) - x
                entries.append(self.get(x_mirrored, y))

        horizontal_mirror = Board(info, entries)

        return min(self, horizontal_mirror)

    def __lt__(self, rhs) -> bool:
        return self.encode() < rhs.encode()

    def drop(self, col) -> Optional[Board]:

        winner = self.winner()
        if winner != Player.NONE:
            return None

        info = self.info

        for y in range(info.v_size):
            if self.get(col, y) == Player.NONE:
                entries = list(self.entries)
                entries[y * info.h_size + col] = self.mover()
                drop_board = Board(info, entries)
                return drop_board

        return None
