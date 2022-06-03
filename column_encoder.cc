
///////////////////////
// column_encoder.cc //
///////////////////////

#include <stdexcept>
#include <algorithm>

#include "constants.h"
#include "player.h"
#include "column_encoder.h"

using namespace std;

ColumnEncoder::ColumnEncoder()
{
    using column = vector<Player>;

    vector<column> to_be_processed;
    to_be_processed.push_back(column());

    while (!to_be_processed.empty())
    {
        column col = to_be_processed.back();
        to_be_processed.pop_back();

        // Calculate column value as a base-3 unsigned value
        // and put it into 'column_index_to_column' vector.
        unsigned col_as_unsigned = 0;
        for (int i = col.size() - 1; i >= 0; --i)
        {
            col_as_unsigned *= 3;
            if (col[i] == Player::A)
            {
                col_as_unsigned += 1;
            }
            else if (col[i] == Player::B)
            {
                col_as_unsigned += 2;
            }
        }

        column_index_to_column.push_back(col_as_unsigned);

        // Check if we can put another chip on top.

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
                col.push_back(Player::A);
                to_be_processed.push_back(col);
                col.pop_back();
                col.push_back(Player::B);
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

unsigned ColumnEncoder::num_entries() const
{
    return column_index_to_column.size();
}

unsigned ColumnEncoder::encode(unsigned column) const
{
    if (column >= column_to_column_index.size())
    {
        throw runtime_error("ColumnEncoder::encode -- bad column");
    }
    return column_to_column_index[column];
}

unsigned ColumnEncoder::decode(unsigned column_index) const
{
    if (column_index >= column_index_to_column.size())
    {
        throw runtime_error("ColumnEncoder::decode -- bad column_index");
    }
    return column_index_to_column[column_index];
}
