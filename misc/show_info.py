#! /usr/bin/env python3

import argparse
import numpy as np
from collections import Counter

def main():

    parser = argparse.ArgumentParser()
    parser.add_argument("filename")
    args = parser.parse_args()

    dtype = np.dtype([("moves", int), ("symmetric", bool), ("outcome",'U1'), ("ply", int), ("count", int)])
    data = np.loadtxt(args.filename, usecols=(1, 3, 5, 7, 9), dtype=dtype)

    full    = Counter()
    reduced = Counter()
    
    for (moves, symmetric, outcome, ply, count) in data:
        if symmetric:
            full   [moves] += count
            reduced[moves] += count
        else:
            full   [moves] += 2 * count
            reduced[moves] += count

    print(" ply                 reduced  reduced (cumulative)                     all      all (cumulative)")
    print("====    ====================  ====================    ====================  ====================")
    reduced_c = 0
    full_c = 0
    for moves in sorted(full.keys()):
        full_c    += full[moves]
        reduced_c += reduced[moves]
        print("{:4d}    {:20d}  {:20d}    {:20d}  {:20d}".format(moves, reduced[moves], reduced_c, full[moves], full_c))
    print()

    if False:

        draw        = 0
        trivial_win = 0
        other       = 0

        for (moves, symmetric, outcome, ply, count) in data:
            if outcome == '-':
                draw += count
            elif ply <= 0:
                trivial_win += count
            else:
                other += count

        print("draw ............. : {:13d}".format(draw))
        print("trivial win ...... : {:13d}".format(trivial_win))
        print("other ............ : {:13d}".format(other))


if __name__ == "__main__":
    main()
