
//////////////////////
// column_encoder.h //
//////////////////////

#ifndef COLUMN_ENCODER_H
#define COLUMN_ENCODER_H

#include <vector>

class ColumnEncoder
{
    public:

        // Instantiate the ColumnEncoder by enumerating all possible
        // columns and generating lookup tables.
        ColumnEncoder();

        unsigned num_entries() const;

        unsigned encode(unsigned column) const;

        unsigned decode(unsigned column_index) const;

    private: // Member variables.

        std::vector<unsigned> column_index_to_column;
        std::vector<unsigned> column_to_column_index;
};

#endif // COLUMN_ENCODER_H
