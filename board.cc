
//////////////
// board.cc //
//////////////

#include <iomanip>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "board.h"
#include "base62.h"

using namespace std;

class ColumnEncoder
{
    public:

        ColumnEncoder()
        {
            using column = vector<unsigned>;

            vector<column> to_be_processed;
            to_be_processed.push_back(column());
            
            while (!to_be_processed.empty())
            {
                column col = to_be_processed.back();
                to_be_processed.pop_back();

                // Calculate column as unsigned value, and put it into 'column_index_to_column' vector.
                unsigned col_as_unsigned = 0;
                for (int i = col.size() -1; i >= 0; --i)
                {
                    col_as_unsigned *= 3;
                    if (col[i] == PLAYER_A)
                    {
                        col_as_unsigned += 1;
                    }
                    else if (col[i] == PLAYER_B)
                    {
                        col_as_unsigned += 2;
                    }                    
                }

                column_index_to_column.push_back(col_as_unsigned);
                
                // Check if we can put another one on top.

                if (col.size() < V_SIZE)
                {
                    bool has_winner = false;

                    if (col.size() >= Q)
                    {
                        has_winner = true;

                        for (int i = 1; i < Q; ++i)
                        {
                            if (col[col.size() - 1] != col[col.size() - 1 - i])
                            {
                                has_winner = false;
                                break;
                            }
                        }
                    }

                    if (!has_winner)
                    {
                        col.push_back(PLAYER_A);
                        to_be_processed.push_back(col);
                        col.pop_back();
                        col.push_back(PLAYER_B);
                        to_be_processed.push_back(col);
                        col.pop_back();
                    }
                }
            }

            sort(column_index_to_column.begin(), column_index_to_column.end());
            
            const unsigned max_col_as_unsigned = column_index_to_column.back();

            column_to_column_index.resize(max_col_as_unsigned + 1);
            for (unsigned i = 1; i < column_index_to_column.size(); ++i)
            {
                column_to_column_index[column_index_to_column[i]] = i;
            }
        }

        unsigned num_entries()
        {
            return column_index_to_column.size();
        }

        unsigned encode(unsigned column) const
        {
            if (column >= column_to_column_index.size())
            {
                throw runtime_error("bad column encode");
            }
            return column_to_column_index[column];
        }

        unsigned decode(unsigned column_index) const
        {
            if (column_index >= column_index_to_column.size())
            {
                throw runtime_error("bad column decode");
            }
            return column_index_to_column[column_index];
        }

    private:

        vector<unsigned> column_index_to_column;
        vector<unsigned> column_to_column_index;
};

static ColumnEncoder column_encoder;

// static method
Board Board::empty()
{
    Board board;

    for (int i = 0; i < V_SIZE * H_SIZE; ++i)
    {
        board.entries[i] = EMPTY;
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
            if (entries[y * H_SIZE + x] == PLAYER_A)
            {
                column += 1;
            }
            else if (entries[y * H_SIZE + x] == PLAYER_B)
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
    // Decode board.
    Board board;

    for (int x = H_SIZE - 1; x >= 0; --x)
    {
        unsigned column = column_encoder.decode(n % column_encoder.num_entries());
        n /= column_encoder.num_entries();

        for (int  y = V_SIZE - 1; y >= 0; --y)
        {
            const unsigned digit = (column % 3);
            board.entries[y * H_SIZE + x] = (digit == 0) ? EMPTY : (digit == 1) ? PLAYER_A : PLAYER_B;
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
        a_min_b += ((entry == PLAYER_A) - (entry == PLAYER_B));
    }
    if (a_min_b == 0)
    {
        return PLAYER_A;
    }
    else if (a_min_b == 1)
    {
        return PLAYER_B;
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

            if (player != EMPTY)
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
                            if (player == PLAYER_A)
                            {
                                player_a_wins = true;
                            }
                            else if (player == PLAYER_B)
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

    if (player_a_wins)
    {
        return PLAYER_A;
    }

    if (player_b_wins)
    {
        return PLAYER_B;
    }

    return EMPTY;
}

vector<Board> Board::generate_boards() const
{
    // *** TO BE RE-IMPLEMENTED **

    // Return a vector of boards that can be reached from the
    // current board state by making a single move.
    //
    // This method does not normalize the boards it generates.

    vector<Board> next_boards;

    if (winner() == EMPTY)
    {
        const Player player = mover();
        for (int x = 0; x < H_SIZE; ++x)
        {
            for (int y = V_SIZE - 1; y >= 0; --y)
            {
                const int i = y * H_SIZE + x;

                if (entries[i] == EMPTY)
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

    // Boards are identical, so (lhs < rhs) is false.
    return false;
}

ostream & operator << (ostream & out, const Board & board)
{
    out << board.to_string(NUM_DIGITS);
    return out;
}

istream & operator >> (istream & in, Board & board)
{
    string s;
    in >> setw(NUM_DIGITS) >> s;
    board = Board::from_string(s);
    return in;
}
