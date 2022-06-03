
//////////////////////
// unique_boards.cc //
//////////////////////

#include <vector>

#include "unique_boards.h"
#include "board_normalizer.h"

using namespace std;

set<Board> generate_unique_boards(const Board & board)
{
    // Note that board_initializer is declared as static; this is done to make sure that
    // the BoardNormalizer is only instantiated once, if and when it is first needed.

    static BoardNormalizer board_normalizer;

    const vector<Board> boards = board.generate_boards();

    set<Board> unique_boards;

    for (const Board & board: boards)
    {
        unique_boards.insert(board_normalizer.normalize(board));
    }

    return unique_boards;
}
