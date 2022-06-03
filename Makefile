
# Note: the CC variable is used to select the linker.

CXXFLAGS = -W -Wall -O3
CC=g++

.PHONY : clean

connect4 : connect4.o base62.o board.o unique_boards.o board_normalizer.o permutation.o files.o

connect4.o         : connect4.cc base62.h board.h unique_boards.h files.h
base62.o           : base62.cc base62.h
board.o            : board.cc board.h base62.h
unique_boards.o    : unique_boards.cc unique_boards.h board.h
board_normalizer.o : board_normalizer.cc board_normalizer.h permutation.h
permutation.o      : permutation.cc permutation.h board.h
files.o            : files.cc files.h

clean :
	$(RM) connect4 connect4.o base62.o board.o unique_boards.o board_normalizer.o permutation.o files.o *~ *.dat
