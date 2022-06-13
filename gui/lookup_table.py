"""Implement a lookup table for connect-4."""
import os
import mmap
from typing import NamedTuple

from board import Board
from player import Player

class Evaluation(NamedTuple):
    player: Player
    winply: int


class LookupTable:

    def __init__(self, filename: str, num_octets_per_entry: int):
        self.filename = filename
        self.num_octets_per_entry = num_octets_per_entry
        self.fd = None
        self.mm = None
        self.size = None

    def open(self):
        self.fd = os.open(self.filename, os.O_RDONLY)

        stat_result = os.fstat(self.fd)
        self.size = stat_result.st_size

        assert self.size % self.num_octets_per_entry == 0

        self.mm = mmap.mmap(self.fd, self.size, mmap.MAP_SHARED, prot = mmap.PROT_READ)

    def close(self):
        self.mm.close()
        self.mm = None
        os.close(self.fd)
        self.fd = None
        self.size = None

    def num_entries(self):
        return self.size // self.num_octets_per_entry

    def get_entry(self, index):
        return self.mm[index * self.num_octets_per_entry: (index + 1) * self.num_octets_per_entry]

    def _lookup_raw_value(self, key: int) -> int:
        a = 0
        b = self.num_entries() - 1

        while a <= b:

            mid = (a + b) // 2

            #print(a, b, mid)

            mid_entry = self.get_entry(mid)
            #print(" ".join("0x{:02x}".format(x) for x in mid_entry))

            mid_key = 0
            for digit in mid_entry[:-1]:
                mid_key *= 256
                mid_key += digit

            if mid_key > key:
                b = mid - 1
                continue

            if mid_key < key:
                a = mid + 1
                continue

            return mid_entry[-1]

        raise KeyError()

    def lookup(self, board: Board):
        raw_value = self._lookup_raw_value(board.normalize().encode())
        if raw_value >= 128:
            return Evaluation(Player.B, 255 - raw_value)
        if raw_value > 0:
            return Evaluation(Player.A, raw_value - 1)
        return Evaluation(Player.NONE, 0)
