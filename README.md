
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
This file gives the number of unique boards (up to reflection), at each generation. This number should correspond
to the number of nodes in each of the node files our program generates.

The C++ source code for the 'connect-4' program consists of 11 files:

* connect4.cc - The toplevel program, containing `main` and the code for the sub-steps.
* constants.h - Basic constants describing the board dimensions, etc.
* board.cc, board.h - The `Board` class and its methods.
* column_encoder.cc, column_encoder.h -- The `ColumnEncoder` class and its methods.
* player.cc, player.h - The `Player` enum class and its methods.
* base62.cc, base62.h - Implement pure-ascii encoding and decoding of 64-bit unsigned integers in 'base-62' format, using only the characters 0-9, A-Z, and a-z.
* files.h - Support specification of file streams by name, with special handling for stdin/stdout.

The C++ program can be compiled and linked using the provided Makefile.

The file 'connect4-script' is a bash script to run a full forward, backward, and combine run for connect-4.

GENERATING THE FULL TREE
------------------------

(TBW)

EXPECTED OUTCOME
----------------

(TBW)
