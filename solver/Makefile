
# Note: the CC variable is used to select the linker.
# We override it to make sure the linker knows how to deal with C++ object files.

CC=$(CXX)
CXXFLAGS = -W -Wall -O3 -std=c++14

.PHONY : clean default run

TARGET  = connect4
OBJECTS = board.o column_encoder.o base62.o score.o outcome.o connect4.o
HEADERS = board.h column_encoder.h base62.h score.h outcome.h player.h board_size.h files.h

default : $(TARGET)
	@echo
	@echo "Target '$(TARGET)' is up-to-date."
	@echo
	@echo "Compile-time constants:"
	@echo
	@$(PWD)/$(TARGET) --print-constants
	@echo

run : $(TARGET)
	./connect4-script

$(TARGET) : $(OBJECTS)

# Instead of doing transitive dependency analysis, we just make all C++ source files depend on all C++ header files.

board.o          : board.cc          $(HEADERS)
column_encoder.o : column_encoder.cc $(HEADERS)
base62.o         : base62.cc         $(HEADERS)
player.o         : player.cc         $(HEADERS)
outcome.o        : outcome.cc        $(HEADERS)
score.o          : score.cc          $(HEADERS)
connect4.o       : connect4.cc       $(HEADERS)

clean :
	$(RM) $(TARGET) $(OBJECTS) *.log *~ *.dat *.bin *.bin.xz
