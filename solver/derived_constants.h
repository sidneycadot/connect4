
/////////////////////////
// derived_constants.h //
/////////////////////////

#ifndef DERIVED_CONSTANTS_H
#define DERIVED_CONSTANTS_H

#include "board_size.h"
#include "number_of_possible_columns.h"

// Determine several constants that define the storage size of a Board when represented in the base-62
// ASCII representation (see base62.h) and the base-256 binary representation.
//
// To calculate those, we need two constexpr functions that enable the compiler to calculate those
// values at compile-time.

constexpr uint64_t power(unsigned a, unsigned b)
{
    return (b == 0) ? 1 : a * power(a, b - 1);
}

constexpr unsigned num_digits_required(unsigned base, uint64_t count)
{
    // Find the smallest value 'digits' such that power(base, digits) >= count.

    return (count <= 1) ? 0 : 1 + num_digits_required(base, (count + base - 1) / base);
}

const unsigned NUMBER_OF_POSSIBLE_COLUMNS = number_of_possible_columns(CONNECT_Q, V_SIZE);
const uint64_t NUMBER_OF_BOARDS_IN_COLUMN_REPRESENTATION = power(NUMBER_OF_POSSIBLE_COLUMNS, H_SIZE);

const unsigned NUM_BASE62_BOARD_DIGITS  = num_digits_required( 62, NUMBER_OF_BOARDS_IN_COLUMN_REPRESENTATION);
const unsigned NUM_BASE256_BOARD_DIGITS = num_digits_required(256, NUMBER_OF_BOARDS_IN_COLUMN_REPRESENTATION);

#endif // DERIVED_CONSTANTS_H
