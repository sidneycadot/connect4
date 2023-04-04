"""Query the connect-4 lookup table."""

import os
from typing import Sequence

from .board import Board
from .simple_types import Outcome, Score


def _from_digits(digits: Sequence[int], base: int):
    """Return the value of a number, given its 'base' digits, least to most significant."""
    return 0 if len(digits) == 0 else _from_digits(digits[1:], base) * base + digits[0]


class LookupTable:

    def __init__(self, filename: str, number_of_octets_per_entry: int):
        self.filename = filename
        self.number_of_octets_per_entry = number_of_octets_per_entry
        self.lookup_table_file = None
        self.size = None

    def open(self) -> None:
        """Open the lookup table file."""
        stat_result = os.stat(self.filename)
        self.size = stat_result.st_size

        if not (self.size % self.number_of_octets_per_entry == 0):
            raise ValueError("Bad size for lookup table.")

        self.lookup_table_file = open(self.filename, "rb")

    def close(self) -> None:
        """Close the lookup table file."""
        self.lookup_table_file.close()
        self.lookup_table_file = None
        self.size = None

    def num_entries(self) -> int:
        """Return number of entries in the table."""
        return self.size // self.number_of_octets_per_entry

    def _get_entry(self, index: int) -> bytes:
        """Get octets corresponding to a certain index."""
        self.lookup_table_file.seek(index * self.number_of_octets_per_entry)
        entry = self.lookup_table_file.read(self.number_of_octets_per_entry)
        assert len(entry) == self.number_of_octets_per_entry
        return entry

    def _lookup_score_octet(self, key: int) -> int:
        """Perform a binary search in the lookup table."""

        a = 0
        b = self.num_entries() - 1

        while a <= b:

            mid_index = (a + b) // 2

            # An entry is a multi-octet key followed by a score octet.
            mid_entry = self._get_entry(mid_index)

            mid_key = int.from_bytes(mid_entry[:-1], 'big')

            if mid_key > key:
                b = mid_index - 1
            elif mid_key < key:
                a = mid_index + 1
            else:
                # Found the key.
                # The last octet represents the game outcome.
                return mid_entry[-1]

        raise KeyError("Key not found in lookup table.")

    @staticmethod
    def _column_encoded(board: Board, col: int) -> int:
        info = board.info
        digits = [board.get(col, y).value for y in range(info.v_size)]
        column_ternary = _from_digits(digits, 3)
        return info.column_ternary_to_column_encoded[column_ternary]

    @staticmethod
    def _board_key(board: Board) -> int:

        info = board.info

        columns = [LookupTable._column_encoded(board, col) for col in range(info.h_size)]
        base = len(info.column_ternary_to_column_encoded)

        v1 = _from_digits(columns, base)
        v2 = _from_digits(columns[::-1], base)

        key = min(v1, v2)

        return key

    def lookup(self, board: Board) -> Score:
        """Look up the Score of a Board."""

        key = LookupTable._board_key(board)

        score_octet = self._lookup_score_octet(key)

        # Unpack score octet to 'Score' value.

        score_octet_outcome_bits = score_octet & 0xc0
        ply = score_octet & 0x3f

        if score_octet_outcome_bits == 0x40:
            outcome = Outcome.A_WINS
        elif score_octet_outcome_bits == 0x80:
            outcome = Outcome.B_WINS
        elif score_octet_outcome_bits == 0x00:
            outcome = Outcome.DRAW
        else:
            # outcome == Outcome.INDETERMINATE -- this should never happen.
            raise RuntimeError("Unexpected outcome: INDETERMINATE.")

        return Score(outcome, ply)
