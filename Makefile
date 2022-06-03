
# Note: the CC variable is used to select the linker.
# We override it to make sure the linker knows how to deal with C++ object files.

CXXFLAGS = -W -Wall -O3 -std=c++14
CC=$(CXX)

.PHONY : clean

TARGET = connect4
OBJECTS = connect4.o base62.o board.o player.o column_encoder.o unique_boards.o board_normalizer.o permutation.o files.o

$(TARGET) : $(OBJECTS)

connect4.o         : connect4.cc base62.h board.h unique_boards.h files.h
base62.o           : base62.cc base62.h
board.o            : board.cc board.h base62.h
column_encoder.o   : column_encoder.cc column_encoder.h
unique_boards.o    : unique_boards.cc unique_boards.h board.h
board_normalizer.o : board_normalizer.cc board_normalizer.h permutation.h
permutation.o      : permutation.cc permutation.h board.h
player.o           : player.cc player.h
files.o            : files.cc files.h

clean :
	$(RM) $(TARGET) $(OBJECTS) *~ *.dat
