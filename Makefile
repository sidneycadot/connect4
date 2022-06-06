
# Note: the CC variable is used to select the linker.
# We override it to make sure the linker knows how to deal with C++ object files.

CC=$(CXX)
CXXFLAGS = -W -Wall -O3 -std=c++14

.PHONY : clean

TARGET  = connect4
OBJECTS = board.o column_encoder.o base62.o player.o connect4.o
HEADERS = board.h column_encoder.h base62.h player.h constants.h files.h

$(TARGET) : $(OBJECTS)

# Instead of doing transitive dependency analysis, we just make all C++ source files depend on all C++ header files.

board.o          : board.cc          $(HEADERS)
column_encoder.o : column_encoder.cc $(HEADERS)
base62.o         : base62.cc         $(HEADERS)
player.o         : player.cc         $(HEADERS)
connect4.o       : connect4.cc       $(HEADERS)

clean :
	$(RM) $(TARGET) $(OBJECTS) *~ *.dat
