#! /usr/bin/env python3

import os

filename_bfile = "b013582.txt"

total = 2266724352362

b_prev = 1
sum_b = 0
with open(filename_bfile) as fi:
    for line in fi:
        (a, b) = map(int, line.split())

        filename = "nodes_{}.dat.xz".format(a)
        #try:
        #    stat_result = os.stat(filename)
        #    print(stat_result)
        #except FileNotFoundError:
        #    pass

        sum_b += b
        print("{:2}  {:12}  x {:8.6f} ; {:5.2f} %  {:13}  {:6.2f} %".format(a, b, b / b_prev, b / total * 100.0, sum_b, sum_b / total * 100.0))

        b_prev = b

