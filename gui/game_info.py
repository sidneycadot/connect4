"""Game information."""

import os
import re
from typing import Dict

from simple_types import Player
from lookup_table import LookupTable


def number_of_digits_required(base, count):
    """Find the smallest value 'digits' such that power(base, digits) >= count."""
    if count <= 1:
        return 0
    else:
        return 1 + number_of_digits_required(base, (count + base - 1) // base)


def make_column_ternary_to_column_encoded(n, q) -> Dict[int, int]:

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


class GameInfo:

    def __init__(self, filename: str):
        self._filename = filename
        self.basename = os.path.basename(self._filename)
        self.connect_q = None
        self.h_size = None
        self.v_size = None
        self.column_ternary_to_column_encoded = None
        self.lookup_table = None
        self._is_open = False

    def __enter__(self):
        self.open()
        return self

    def __exit__(self, exception_type, exception_value, exception_traceback):
        self.close()

    def open(self):

        if self._is_open:
            raise RuntimeError("Attempt to open a GameInfo instance that is already open.")

        regexp = "^connect([0-9]+)_([0-9]+)x([0-9]+).new$"
        match = re.match(regexp, self.basename)

        if match is None:
            raise ValueError("Bad filename")

        (connect_q, h_size, v_size) = map(int, match.groups())

        column_ternary_to_column_encoded = make_column_ternary_to_column_encoded(v_size, connect_q)

        number_of_possible_columns = len(column_ternary_to_column_encoded)
        board_representation_octets = number_of_digits_required(256, number_of_possible_columns ** h_size)

        octets_per_lut_entry = board_representation_octets + 1

        lookup_table = LookupTable(self._filename, octets_per_lut_entry)
        lookup_table.open()

        self.connect_q = connect_q
        self.h_size = h_size
        self.v_size = v_size
        self.column_ternary_to_column_encoded = column_ternary_to_column_encoded
        self.lookup_table = lookup_table

        self._is_open = True

    def close(self):

        if not self._is_open:
            raise RuntimeError("Attempt to close a GameInfo instance that is already closed.")

        self.lookup_table.close()

        self._is_open = False
