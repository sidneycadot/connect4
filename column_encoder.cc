
///////////////////////
// column_encoder.cc //
///////////////////////

#include <stdexcept>
#include <algorithm>

#include "board_size.h"
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
        unsigned column_ternary = 0;
        for (int i = col.size() - 1; i >= 0; --i)
        {
            column_ternary *= 3;
            if (col[i] == Player::A)
            {
                column_ternary += 1;
            }
            else if (col[i] == Player::B)
            {
                column_ternary += 2;
            }
        }

        column_encoded_to_column_ternary.push_back(column_ternary);

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

    // Sort all possible encoded columns.
    sort(column_encoded_to_column_ternary.begin(), column_encoded_to_column_ternary.end());

    // The highest possible ternary column number we can encounter.
    const unsigned max_column_ternary = column_encoded_to_column_ternary.back();

    column_ternary_to_column_encoded.resize(max_column_ternary + 1);
    for (unsigned i = 0; i < column_encoded_to_column_ternary.size(); ++i)
    {
        column_ternary_to_column_encoded[column_encoded_to_column_ternary[i]] = i;
    }
}
