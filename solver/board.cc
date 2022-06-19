
//////////////
// board.cc //
//////////////

#include <iomanip>
#include <stdexcept>
#include <algorithm>

#include "base62.h"
#include "board.h"

using namespace std;

// Definition of the static column encoder member.
ColumnEncoder Board::column_encoder;

// static method
Board Board::empty()
{
    Board board;

    for (int y = 0; y < V_SIZE; ++y)
    {
        for (int x = 0; x < H_SIZE; ++x)
        {
            board.entries[y][x] = Player::NONE;
        }
    }
    return board;
}

uint64_t Board::to_uint64() const
{
    uint64_t n = 0;
    for (int x = 0; x < H_SIZE; ++x)
    {
        unsigned column = 0;
        for (int y = 0; y < V_SIZE; ++y)
        {
            column *= 3;
            switch (entries[y][x])
            {
                case Player::NONE : column += 0; break;
                case Player::A    : column += 1; break;
                case Player::B    : column += 2; break;
            }
        }

        n *= column_encoder.num_entries();
        n += column_encoder.encode(column);
    }
    return n;
}

// static method
Board Board::from_uint64(uint64_t n)
{
    Board board;

    for (int x = H_SIZE - 1; x >= 0; --x)
    {
        unsigned column = column_encoder.decode(n % column_encoder.num_entries());
        n /= column_encoder.num_entries();

        for (int  y = V_SIZE - 1; y >= 0; --y)
        {
            const unsigned digit = (column % 3);
            board.entries[y][x] = (digit == 0) ? Player::NONE : (digit == 1) ? Player::A : Player::B;
            column /= 3;
        }
    }
    return board;
}

string Board::to_base62_string() const
{
    return uint64_to_base62_string(to_uint64(), NUM_BASE62_BOARD_DIGITS);
}

// static method
Board Board::from_base62_string(const string & s)
{
    return from_uint64(base62_string_to_uint64(s));
}

Player Board::mover() const
{
    // Determine whether player A or B has the move.
    int a_min_b = 0;

    for (int y = 0; y < V_SIZE; ++y)
    {
        for (int x = 0; x < H_SIZE; ++x)
        {
            const Player entry = entries[y][x];
            a_min_b += ((entry == Player::A) - (entry == Player::B));
        }
    }

    if (a_min_b == 0)
    {
        return Player::A;
    }
    else if (a_min_b == 1)
    {
        return Player::B;
    }
    else
    {
        throw runtime_error("Board::mover: impossible board detected.");
    }
}


unsigned Board::count() const
{
    // TODO: we can get rid of this method at some point.
    unsigned counter = 0;

    for (int y = 0; y < V_SIZE; ++y)
    {
        for (int x = 0; x < H_SIZE; ++x)
        {
            if (entries[y][x] != Player::NONE)
            {
                ++counter;
            }
        }
    }

    return counter;
}

// static method
bool Board::is_valid_coordinate(int x, int y)
{
    return (0 <= x) && (x < H_SIZE) && (0 <= y) && (y < V_SIZE);
}

Score Board::score() const
{
    // Find the score of the Board if it can be determined by direct inspection.

    bool player_a_wins = false;
    bool player_b_wins = false;

    for (int y = 0; y < V_SIZE; ++y)
    {
        for (int x = 0; x < H_SIZE; ++x)
        {
            const Player player = entries[y][x];

            if (player != Player::NONE)
            {
                const int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

                for (int d = 0; d < 4; ++d)
                {
                    const int dx = directions[d][0];
                    const int dy = directions[d][1];

                    if (is_valid_coordinate(x + dx * (Q - 1), y + dy * (Q - 1)))
                    {
                        bool found_win = true;
                        for (int i = 1; i < Q; ++i)
                        {
                            if (entries[y + dy * i][x + dx * i] != player)
                            {
                                found_win = false;
                            }
                        }
                        if (found_win)
                        {
                            // Found a winning stretch!
                            switch (player)
                            {
                                case Player::A : player_a_wins = true; break;
                                case Player::B : player_b_wins = true; break;
                                default        : throw runtime_error("Board::score: bad winner");
                            }
                        } // found winning stretch
                    } // coordinates in the stretch are all valid
                } // examine all 4 directions
            } // the initial cell is not empty
        } // walk x
    } // walk y

    if (player_a_wins && player_b_wins)
    {
        throw runtime_error("Board::score: multiple winners found, which is impossible.");
    }

    const Outcome outcome = player_a_wins ? Outcome::A_WINS : player_b_wins ? Outcome::B_WINS : full() ? Outcome::DRAW : Outcome::INDETERMINATE;

    return Score(outcome, 0);
}

Board Board::normalize() const
{
    Board horizontal_mirror;

    for (int y = 0; y < V_SIZE; ++y)
    {
        for (int x = 0; x < H_SIZE; ++x)
        {
            const int x_mirrored = (H_SIZE - 1) - x;
            horizontal_mirror.entries[y][x_mirrored] = entries[y][x];
        }
    }

    return min(*this, horizontal_mirror);
}

set<Board> Board::generate_unique_normalized_boards() const
{
    // Return a set of normalized boards that can be reached from the
    // current board state by making a single move.

    set<Board> next_boards;

    if (score().outcome == Outcome::INDETERMINATE)
    {
        const Player player = mover();
        for (int x = 0; x < H_SIZE; ++x)
        {
            for (int y = V_SIZE - 1; y >= 0; --y)
            {
                if (entries[y][x] == Player::NONE)
                {
                    Board next_board(*this);
                    next_board.entries[y][x] = player;
                    next_boards.insert(next_board.normalize());
                    break;
                }
            }
        }
    }

    return next_boards;
}

bool Board::full() const
{
    for (int x = 0; x < H_SIZE; ++x)
    {
        if (entries[0][x] == Player::NONE)
        {
            return false;
        }
    }
    return true;
}

bool operator < (const Board & lhs, const Board & rhs)
{
    // We compare boards according to their representation as uint64 values.
    return lhs.to_uint64() < rhs.to_uint64();
}

ostream & operator << (ostream & out, const Board & board)
{
    out << board.to_base62_string();
    return out;
}

istream & operator >> (istream & in, Board & board)
{
    string s;
    in >> setw(NUM_BASE62_BOARD_DIGITS) >> s;
    board = Board::from_base62_string(s);
    return in;
}
