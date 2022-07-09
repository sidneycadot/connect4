
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
    // Looking at a single column of height n, we find that not all length-n sequences of
    // {A, B, empty} are valid columns:
    //
    //   * Player chips cannot float above empty positions, so the player chips must be bunched up
    //     at the bottom of the column.
    //
    //   * If a sequence of 'q' chips of either player A or B occur in the column, they must be in
    //     the top-most 'q' non-empty positions, since the occurrence of 'q' identical non-empty
    //     chips ends the game immediately with a win for the player with that chip color.
    //
    // We can use these rules to enumerate all possible columns, given the height n and a win rule
    // 'q-in-a-row'. For example, it turns out there are precisely 111 valid columns in the standard
    // connect-4 game with n=6 and q=4.
    //
    // The `ColumnEncoder` class enumerates all valid column configurations in its constructor, and
    // provides 'encode' and 'decode' methods to convert between ternary-encoded columns and a compect
    // encoding as an unsigned integer. This latter representation allows for the compact storage of
    // the state of a board as a sequence of valid columns.

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
