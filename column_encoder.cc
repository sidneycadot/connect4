
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

        // Represent the column as a base-3 unsigned value.
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

    // We enumerated the ternary representation of all valid columns.
    // Sort them; the end result of this is the final encoded-to-ternary lookup table.
    sort(column_encoded_to_column_ternary.begin(), column_encoded_to_column_ternary.end());

    // Shrink the 'column_encoded_to_column_ternary' vector to minimize its memory footprint.
    column_encoded_to_column_ternary.shrink_to_fit();

    // Find the highest valid ternary column value from the lookup table.

    const unsigned max_column_ternary = column_encoded_to_column_ternary.back();

    column_ternary_to_column_encoded.resize(max_column_ternary + 1);

    // Construct the inverse (ternary-to-encoded) lookup table.

    for (unsigned i = 0; i < column_encoded_to_column_ternary.size(); ++i)
    {
        column_ternary_to_column_encoded[column_encoded_to_column_ternary[i]] = i;
    }
}
