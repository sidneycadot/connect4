
# Note: the CC variable is used to select the linker.
# We override it to make sure the linker knows how to deal with C++ object files.

CXXFLAGS = -W -Wall -O3 -std=c++14
CC=$(CXX)
LDFLAGS = -g

.PHONY : clean

TARGET  = connect4
OBJECTS = connect4.o base62.o board.o player.o column_encoder.o board_normalizer.o permutation.o
HEADERS = base62.h board.h board_normalizer.h column_encoder.h constants.h files.h permutation.h player.h

$(TARGET) : $(OBJECTS)

# Instead of doing transitive dependency analysis, we just pretend that all C++ source files depend on all C++ header files.
# This simplifies our header file drastically.

connect4.o         : connect4.cc         $(HEADERS)
board_normalizer.o : board_normalizer.cc $(HEADERS)
permutation.o      : permutation.cc      $(HEADERS)
board.o            : board.cc            $(HEADERS)
column_encoder.o   : column_encoder.cc   $(HEADERS)
base62.o           : base62.cc           $(HEADERS)
player.o           : player.cc           $(HEADERS)

clean :
	$(RM) $(TARGET) $(OBJECTS) *~ *.dat
