
CONNECT-4 SOLVER
================

INTRODUCTION
------------

(TBW)

ALGORITHM DESCRIPTION
---------------------

(TBW)

LIST OF FILES
-------------

The file README.md contains background information on this code. You are reading it right now.

The file 'show_gametree_size.py' is a Python script that shows the connect-4 gametree size at each generation.
It uses the file 'b013582.txt', as downloaded from the Online Encyclopedia of Integer Sequences (OEIS) as a reference.
This file gives the number of unique boards (up to reflection), at each generation.

The C++ source code for the 'connect-4' program consists of 13 files:

* connect4.cc - The toplevel program, containing `main` and the code for the sub-steps.
* board.cc, board.h - The `Board` class and its methods.
* unique_boards.cc unique_boards.h - Convert a vector of Boards to a set of unique, normalized boards.
* board_normalizer.cc, board_normalizer.h - Implement the `BoardNormalizer` class, which is needed by the `generate_unique_boards` function.
* permutation.cc, permutation.h - Implement the `Permuation` class used by board normalization.
* base62.cc, base62.h - Implement pure-ascii encoding and decoding of 64-bit unsigned integers in 'base-62' format,
                        using only the characters 0-9, A-Z, and a-z.
* files.cc, files.h - Support specification of file streams by name, with special handling for stdin/stdout.

The C++ program can be compiled and linked using the provided Makefile.

The file 'connect4-script' is a bash script to run a full forward, backward, and combine run for connect-4.

GENERATING THE FULL TREE
------------------------

(TBW)

EXPECTED OUTCOME
----------------

(TBW)
