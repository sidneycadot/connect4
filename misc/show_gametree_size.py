#! /usr/bin/env python3

import sys
import os

script_directory = os.path.dirname(sys.argv[0])
filename_bfile = os.path.join(script_directory, "b013582.txt")

total = 2266724352362

b_prev = 1
sum_b = 0
with open(filename_bfile) as fi:
    for line in fi:
        (a, b) = map(int, line.split())
        sum_b += b
        print("{:2}  {:12}  x {:8.6f} ; {:5.2f} %  {:13}  {:6.2f} %".format(a, b, b / b_prev, b / total * 100.0, sum_b, sum_b / total * 100.0))
        b_prev = b
