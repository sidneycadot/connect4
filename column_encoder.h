
//////////////////////
// column_encoder.h //
//////////////////////

#ifndef COLUMN_ENCODER_H
#define COLUMN_ENCODER_H

#include <vector>

class ColumnEncoder
{
    // In the game of connect-4, each board field can be in one of three states: occupied by player A,
    // occupied by player B, or empty.
    //
    // However, not all possible columns are valid. Specifically, non-empty fields can not exist above
    // empty fields; and all fields above a connect-4 must be empty, since a connect-4 immediately
    // ends the game.
    //
    // The `ColumnEncoder` class determines all valid column configurations and provides functions to
    // change between ternary-encoded columns and a encoding as an unsigned integer that is more
    // efficient.

    public:

        // Initialize the ColumnEncoder by enumerating all valid
        // columns and generating bidirectional lookup tables.
        ColumnEncoder();

        // Return the number of different valid columns.
        unsigned num_entries() const
        {
            return column_encoded_to_column_ternary.size();
        }

        // Encode a column, expressed as a ternary number, to its encoded form.
        unsigned encode(unsigned column_ternary) const
        {
            return column_ternary_to_column_encoded.at(column_ternary);
        }

        // Decode a column, expressed as an encoded form, to a ternary number.
        unsigned decode(unsigned column_encoded) const
        {
            return column_encoded_to_column_ternary.at(column_encoded);
        }

    private: // Member variables.

        std::vector<unsigned> column_encoded_to_column_ternary;
        std::vector<unsigned> column_ternary_to_column_encoded;
};

#endif // COLUMN_ENCODER_H
