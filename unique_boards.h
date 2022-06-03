
/////////////////////
// unique_boards.h //
/////////////////////

#ifndef UNIQUE_BOARDS_H
#define UNIQUE_BOARDS_H

#include <set>

#include "board.h"

std::set<Board> generate_unique_boards(const Board & board);

#endif // UNIQUE_BOARDS_H
