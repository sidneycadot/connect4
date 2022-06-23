
//////////////////////
// column_encoder.h //
//////////////////////

#ifndef COLUMN_ENCODER_H
#define COLUMN_ENCODER_H

#include <vector>

class ColumnEncoder
{
    // In the game of connect-4, each board entry can be in one of three states: occupied by a
    // player A chip, occupied by a player B chip, or empty.
    //
    // However, not all possible columns are valid. Non-empty fields cannot occur above empty fields;
    // and all fields above a connect-4 must be empty, since a connect-4 immediately ends the game.
    //
    // The `ColumnEncoder` class enumerates all valid column configurations and provides functions to
    // convert between ternary-encoded columns and a compact encoding as an unsigned integer.
    // The latter representation allows for the compact storage of the state of a board.

    public:

        // Initialize the ColumnEncoder by enumerating all valid columns
        // and generating bi-directional lookup tables.
        ColumnEncoder();

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
