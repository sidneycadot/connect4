
/////////////
// board.h //
/////////////

#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <string>
#include <set>
#include <istream>
#include <ostream>

#include "column_encoder.h"
#include "player.h"
#include "score.h"
#include "board_size.h"

class Board
{
    public:

        // Make an empty board.
        static Board empty();

        // Determine which player has the move.
        Player mover() const;

        // Determine if the board has a connect-Q and, if so, for which player.
        // If the board is full and there is no winner, the outcome will we DRAW.
        // Otherwise, the output will be INDETERMINATE.
        Outcome trivial_outcome() const;

        // Count the number of occupied board entries, which is identical to the move number.
        unsigned count() const;

        // Check if the Board is full.
        bool full() const;

        // Normalize the board (i.e., return the smallest board, identical up to horizontal reflection).
        Board normalize() const;

        // Generate the set of normalized Boards that are reachable from this Board with a single move.
        std::set<Board> generate_unique_normalized_boards() const;

        // Encode the Board as a 64-bit unsigned integer.
        uint64_t to_uint64() const;

        // Decode a Board from a 64-bit unsigned integer.
        static Board from_uint64(uint64_t n);

        // Encode the Board as a base-62 string.
        std::string to_base62_string() const;

        // Decode a Board from a base-62 string.
        static Board from_base62_string(const std::string & s);

    private: // Member functions.

        // Check if the x and y coordinates are both valid.
        static bool is_valid_coordinate(int x, int y);

    private: // Member variables.

        // The ColumnEncoder supports encoding and decoding the Board to a
        // compact representation, by only considering possible columns.
        static ColumnEncoder column_encoder;

        // The entries on the Board.
        // The first row (i.e., entries[0][0 .. H_SIZE - 1]) is the top row;
        // the last row (i.e., entries[V_SIZE - 1][0 .. H_SIZE - 1]) is the bottom row.
        Player entries[V_SIZE][H_SIZE];

    // Friends functions.

        friend bool operator < (const Board & lhs, const Board & rhs);

        friend std::ostream & operator << (std::ostream & out, const Board & board);

        friend std::istream & operator >> (std::istream & in, Board & board);
};

#endif // BOARD_H
