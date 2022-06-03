
//////////////
// board.cc //
//////////////

#include <iomanip>
#include <stdexcept>

#include "board.h"
#include "base62.h"

using namespace std;

// Definition of the column encoder.
ColumnEncoder Board::column_encoder;

// static method
Board Board::empty()
{
    Board board;

    for (int i = 0; i < V_SIZE * H_SIZE; ++i)
    {
        board.entries[i] = Player::NONE;
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
            if (entries[y * H_SIZE + x] == Player::A)
            {
                column += 1;
            }
            else if (entries[y * H_SIZE + x] == Player::B)
            {
                column += 2;
            }
        }

        n *= column_encoder.num_entries();
        n += column_encoder.encode(column);
    }
    return n;
}

string Board::to_string(unsigned num_digits) const
{
    return uint64_to_base62_string(to_uint64(), num_digits);
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
            board.entries[y * H_SIZE + x] = (digit == 0) ? Player::NONE : (digit == 1) ? Player::A : Player::B;
            column /= 3;
        }
    }
    return board;
}

// static method
Board Board::from_string(const string & s)
{
    return from_uint64(base62_string_to_uint64(s));
}

Player Board::mover() const
{
    // Determine whether player A or B has the move.
    int a_min_b = 0;
    for (int i = 0; i < V_SIZE * H_SIZE; ++i)
    {
        const Player entry = entries[i];
        a_min_b += ((entry == Player::A) - (entry == Player::B));
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
        throw runtime_error("Board::mover: bad mover");
    }
}

// static method
bool Board::is_valid_coordinate(int x, int y)
{
    return (0 <= x) && (x < H_SIZE) && (0 <= y) && (y < V_SIZE);
}

Player Board::winner() const
{
    bool player_a_wins = false;
    bool player_b_wins = false;

    for (int y = 0; y < V_SIZE; ++y)
    {
        for (int x = 0; x < H_SIZE; ++x)
        {
            const Player player = entries[y * H_SIZE + x];

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
                            if (entries[(y + dy * i) * H_SIZE + (x + dx * i)] != player)
                            {
                                found_win = false;
                            }
                        }
                        if (found_win)
                        {
                            // Found a winning stretch!
                            if (player == Player::A)
                            {
                                player_a_wins = true;
                            }
                            else if (player == Player::B)
                            {
                                player_b_wins = true;
                            }
                            else
                            {
                                throw runtime_error("bad winner");
                            }
                        } // found winning stretch
                    } // coordinates in the stretch are all valid
                } // examine all 4 directions
            } // the initial cell is not empty
        } // walk x
    } // walk y

    if (player_a_wins && player_b_wins)
    {
        throw runtime_error("Board::winner: multiple winners");
    }

    return player_a_wins ? Player::A : player_b_wins ? Player::B : Player::NONE;
}

vector<Board> Board::generate_boards() const
{
    // Return a vector of boards that can be reached from the
    // current board state by making a single move.
    //
    // This method does not normalize the boards it generates.

    vector<Board> next_boards;

    if (winner() == Player::NONE)
    {
        const Player player = mover();
        for (int x = 0; x < H_SIZE; ++x)
        {
            for (int y = V_SIZE - 1; y >= 0; --y)
            {
                const int i = y * H_SIZE + x;

                if (entries[i] == Player::NONE)
                {
                    Board next_board(*this);
                    next_board.entries[i] = player;
                    next_boards.push_back(next_board);
                    break;
                }
            }
        }
    }
    return next_boards;
}

bool operator < (const Board & lhs, const Board & rhs)
{
    for (int i = 0; i < V_SIZE * H_SIZE; ++i)
    {
        if (lhs.entries[i] < rhs.entries[i])
        {
            return true;
        }
        if (lhs.entries[i] > rhs.entries[i])
        {
            return false;
        }
    }

    // The boards are identical, so (lhs < rhs) is false.
    return false;
}

ostream & operator << (ostream & out, const Board & board)
{
    out << board.to_string(NUM_BASE62_DIGITS);
    return out;
}

istream & operator >> (istream & in, Board & board)
{
    string s;
    in >> setw(NUM_BASE62_DIGITS) >> s;
    board = Board::from_string(s);
    return in;
}
