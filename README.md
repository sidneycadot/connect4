
CONNECT-4 SOLVER
================

INTRODUCTION
------------

This repository contains the code needed to fully enumerate all game states for
the game connect-4, and to determine, for each of those game states, their
game-theoretical value.

LIST OF FILES
-------------

The file README.md contains background information on this code.
You are reading it right now.

The LICENSE file contains the MIT license applicable to this code.

The file 'show_gametree_size.py' is a Python script that shows the connect-4
gametree size at each generation. It uses the file 'b013582.txt', as obtained
from the Online Encyclopedia of Integer Sequences (OEIS) as a reference.
This file gives the number of unique boards (up to reflection), at each
generation. This number should correspond to the number of nodes in each of
the node files our program generates.

The C++ source code for the 'connect-4' program consists of 12 files:

* connect4.cc - The toplevel program, containing `main` and the code for the sub-steps.
* board_size.h - Constants describing the board dimensions and the encoding widths that follow from that.
* number_of_columns.h - Provides a function that can calculate the number of possible columns at compile time.
* board.cc, board.h - The `Board` class that represents a single Board, with some convenientt methods.
* column_encoder.cc, column_encoder.h - The `ColumnEncoder` class and its methods.
* player.cc, player.h - The `Player` enum class.
* base62.cc, base62.h - Implement a pure-ASCII encoding and decoding of 64-bit unsigned integers in 'base-62' format, using only the characters 0-9, A-Z, and a-z. We need to be able to represent boards as ASCII strings since we heavily rely on the 'sort' utility that cannot sort binary data.
* files.h - Support specification of file streams by name, with special handling for stdin/stdout.

The C++ program can be compiled and linked using the provided Makefile.

The file 'connect4-script' is a bash script to run a full forward, backward,
and combine run for connect-4.

ALGORITHM DESCRIPTION
---------------------

The algorithm proceeds in two main stages: "forward" and "backward".

In the forward stage, all reachable board states are enumerated, starting at
the initial (empty) board, and progressing by making moves. If boards have a
connect-4 (i.e., they are won for one of the player), they are marked a "won
in zero plies for player X (A or B)". The data generated is collected in files
that hold all possible board states after N moves.

In the backward stage, we progress from the end (boards with the highest
possible  number of moves) to the start (the board with no moves, i.e., the
initial game state). Along the way, we add the game-theoretical value of each
board visited, by inspecting the boards that can be reached from it (and that
were previously visited and annotated as such). The end result is that all
board states will be annotated with their game-theoretical value: either "won
in P plies for player X (A or B)" or "not winnable by either player if optimal
play is assumed", i.e., a draw.

The ability to perform both the forward and backward stages relies heavily on
the use of the standard command-line tool 'sort' to do sorting and
de-duplication of ASCII text files, even if those files are *huge*, by using
many temporarily sorted files (tens of millions, while solving the standard
7x6 connect-4 board) and merge-sorting them to produce a single sorted output.
This use of sort is an alternative to using some form of indexing when we need
the ability to look up the game-theoretical valuation of board states that can
be reached by a single move from a certain board.

See the "connect4-script" Bash script for details.
