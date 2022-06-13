
CONNECT4 solver and perfect-play GUI
====================================

This repository contains code to strongly solve the game of CONNECT-4.

The "solver" subdirectory contains the code that generates a perfect-play
database for a certain board size. Note that the amount of storage (tens
of terabytes) and calculation time (months of calculation) required for
the standard 7x6 board is prohibitively large for all but the most dedicated
CONNECT-4 afficianados. The author is currently calculating a full 7x6
solution that will be made publicly available as soon as it is done.

Until that time, it is recommended to experiment with the 6x5 version that
can be generated in a few hours or downloaded. See the README.md file in
the "gui" subdirectory for details.

The "gui" subdirectory contains the PyQt-based GUI that allows play against
the perfect-play database, showing the consequence of each move. For now,
this is based on the 6x5 board.
