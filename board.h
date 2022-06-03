
/////////////
// board.h //
/////////////

#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <string>
#include <vector>
#include <istream>
#include <ostream>

const int V_SIZE = 6; // vertical board size
const int H_SIZE = 7; // horizontal board size
const int Q      = 4; // number of consecutive horizontal/diagonal/vertical cells required to win.

const unsigned NUM_DIGITS = 8; // We need 8 base-62 characters to encode a single board state: ceiling(log(111**7) / log(62))

using Player = char;

const Player EMPTY    = '.';
const Player PLAYER_A = 'A';
const Player PLAYER_B = 'B';

class Board
{
    public:

        // Make an empty board.
        static Board empty();

        // Determine which player has the move.
        Player mover() const;

        // Determine if the game has a connect-4 and if so, for which player.
        Player winner() const;

        // Generate that are reachable from the current board.
        std::vector<Board> generate_boards() const;

        // Encode Board as a base-62 string.
        std::string to_string(unsigned num_digits) const;

        // Decode Board from a base-62 string.
        static Board from_string(const std::string & s);

    private: // Member functions.

        // Check if x and y coordinates are both valid.
        static bool is_valid_coordinate(int x, int y);

        // Encode Board as a 64-bit unsigned integer.
        uint64_t to_uint64() const;

        // Decode Board from a 64-bit unsigned integer.
        static Board from_uint64(uint64_t n);

    private: // Member variables.

        Player entries[V_SIZE * H_SIZE];

    // Friends.

        friend bool operator < (const Board & lhs, const Board & rhs);

        friend std::ostream & operator << (std::ostream & out, const Board & board);

        friend std::istream & operator >> (std::istream & in, Board & board);

        friend class Permutation;
};

#endif // BOARD_H
