
CONNECT4 solver and perfect-play GUI
====================================

This repository contains code to strongly solve the game of CONNECT-4.

The "solver" subdirectory contains the code that generates a perfect-play
database for a certain board size. Note that the amount of storage (tens
of terabytes) and calculation time (months of calculation) required for
the standard 7x6 board is prohibitively large for all but the most dedicated
CONNECT-4 aficionados.

The "7x6" directory contains information specifically about the solution
of the 7x6 board, in particular it documents the the database storage
format, and how to to obtain a premade version of the database.

The "clients" subdirectory contains a command-line interface program that
allows play against the perfect-play database, with the ability to show
the consequence of each move.
