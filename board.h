
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
#include "board_size.h"

class Board
{
    public:

        // Make an empty board.
        static Board empty();

        // Determine which player has the move.
        Player mover() const;

        // Determine if the game has a connect-4 and, if so, for which player.
        Player winner() const;

        // Normalize the board (i.e., return the smallest board, identical up to symmetry).
        Board normalize() const;

        // Generate a set of normalized Boards that are directly reachable from this Board.
        std::set<Board> generate_unique_normalized_boards() const;

        // Encode the Board as a 64-bit unsigned integer.
        uint64_t to_uint64() const;

        // Decode a Board from a 64-bit unsigned integer.
        static Board from_uint64(uint64_t n);

        // Encode the Board as a base-62 string.
        std::string to_string(unsigned num_digits) const;

        // Decode a Board from a base-62 string.
        static Board from_string(const std::string & s);

    private: // Member functions.

        // Check if x and y coordinates are both valid.
        static bool is_valid_coordinate(int x, int y);

    private: // Member variables.

        static ColumnEncoder column_encoder;

        Player entries[V_SIZE * H_SIZE];

    // Friends.

        friend bool operator < (const Board & lhs, const Board & rhs);

        friend std::ostream & operator << (std::ostream & out, const Board & board);

        friend std::istream & operator >> (std::istream & in, Board & board);

        friend class Permutation;
};

#endif // BOARD_H
