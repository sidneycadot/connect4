
CONNECT-4 SOLVER
================

INTRODUCTION
------------

This directory contains the code needed to fully enumerate all game states for
the game connect-4 (and its generalization 'connect-q'), and to determine, for
each of those game states, their game-theoretical value.

This implementation can be parametrized to solve different connect-4 "like"
games, by varying the board size and the number of connnected chips needed
to win. See "board_size.h" for details.

LIST OF FILES
-------------

The file README.md contains background information on the solver code.
You are reading it right now.

The file 'show_gametree_size.py' is a Python script that shows the
connect-4 gametree size at each generation on the standard 7x6 board. It uses
the file 'b013582.txt', as obtained from the Online Encyclopedia of Integer
Sequences (OEIS) as a reference. This file gives the number of unique boards
(up to reflection), at each generation. This number should correspond to the
number of nodes in each of the node files our program generates for the
standard 7x6 connect-4 board.

The file 'connect4-script' is a Bash script to run a full forward, backward,
and combine run for connect-4. It uses a combination of command-line tools,
in particular 'sort', and a custom C++ program called 'connect-4' that can
generate and process game tree nodes in a way that allows strong solution of
the game.

The C++ source code for the 'connect-4' program consists of 16 files:

* connect4.cc - The toplevel program, containing `main` and the code for the sub-steps.
* board_size.h - Constants that define the board dimensions and the win rule ("connect by q").
* derived_constants.h - Compile-time calculated constants for the encoding widths that follow from the board size constants.
* number_of_columns.h - Provides a compile-time function that calculates the number of possible columns.
* column_encoder.cc, column_encoder.h - The `ColumnEncoder` class and its methods.
* board.cc, board.h - The `Board` class that represents a single Board, with some convenient methods.
* outcome.cc, outcome.h - The `Outcome` enum class represent the game-theoretical value of a board position.
* score.cc, score.h - The `Score` class represent the game-theoretical outcome of a board position, including the number of moves to get there.
* player.h - The `Player` enum class represents a player (A / B / NONE).
* base62.cc, base62.h - Implement a pure-ASCII encoding and decoding of 64-bit unsigned integers in 'base-62' format, using only the characters 0-9, A-Z, and a-z. We need to be able to represent boards as ASCII strings since we heavily rely on the 'sort' utility that cannot sort binary data.
* files.h - Support specification of file streams by name, with special handling for stdin/stdout.

The C++ program can be compiled and linked using the provided Makefile.

ALGORITHM DESCRIPTION
---------------------

The algorithm proceeds in two main stages: "forward" and "backward".

In the forward stage, all reachable board states are enumerated, starting at
the initial (empty) board, and progressing by making moves. If boards have a
connect-4 (i.e., they are won for one of the player), they are marked as "won
in zero plies for player X (A or B)". The data generated is collected in files
that hold all possible board states after a given number of moves.

In the backward stage, we progress from the end (boards with the highest
possible number of moves) to the start (the board with no moves, i.e., the
initial game state). Along the way, we add the game-theoretical value of each
board visited, by inspecting the boards that can be reached from it (and that
were previously visited and annotated as such). The end result is that all
board states will be annotated with their game-theoretical value: either "won
in P plies for player X (A or B)" or "not winnable by either player if optimal
play is assumed, and reaching a draw in P plies", i.e., a draw.

The ability to perform both the forward and backward stages relies heavily on
the use of the standard command-line tool 'sort' to do sorting and
de-duplication of ASCII text files, even if those files are *huge*, by using
many temporarily sorted files (tens of millions, while solving the standard
7x6 connect-4 board) and performing an external merge-sort to produce a single
sorted output. This use of sort is an alternative to using some form of
indexing when we need the ability to look up the game-theoretical valuation of
board states that can be reached by a single move from a certain board.

See the "connect4-script" Bash script for details.

RUNNING THE SOLVER
------------------

To generate a solution database, you first need to configure the board size and
the win-rule for the game that you want to solve. You do this by editing the
"board_size.h" file.

Next, generate the `connect4` executable. This can be done by executing make.

Then set the environment variables DATADIR and TMPDIR. The former is where
all generated files will be stored. The latter is where `sort` will store its
temporary files. You can improve performance by having these locations on
separate disks.

Depending on your physical system memory, you may want to edit the SORTBUFx1
and SORTBUFx2 variables that are set in `connect4-script`. Larger values
allow `sort` to use more physical memory for its operations, and lessen
its dependence of temporary disk files. SORTBUGF sizes of at least several
gigabytes are recommended.

Finally, you can run the `connect4-script` Bash script, that will perform
a full set of forward, backward, and merge steps to generate a solution
database.

Be advised that a full run for the standard connect-4 7x6 board will take
months, and requires tens of terabytes of disk space to be available in
both the TEMPDIR and DATADIR locations.

The end result of the script's run will be a binary file ending in `.new`,
containing all reachable board states and their game-theoretical outcome,
including the number of moves until the game ends with optimal play from
both sides. (The winning side will seek to minimize the number of moves
until the win is on the board, whereas the losing side will seek to
maximize the number of moves untill the loss is on the board.)

Since the game files can become huge, some effort was expended to find
optimal compression settings for these files using the `xz` tool. See the
comments at the end of `connect4-script` for guidance.
