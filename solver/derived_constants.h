
/////////////////////////
// derived_constants.h //
/////////////////////////

#ifndef DERIVED_CONSTANTS_H
#define DERIVED_CONSTANTS_H

#include "board_size.h"
#include "number_of_possible_columns.h"

// Determine several constants:
//
// * A constant that holds the number of possible distinct columns, and the number
//   of possible representable boards in a representation based on those;
//
// * Constants that define the storage size of a Board when represented in the base-62
//   ASCII representation (see base62.h) and the base-256 binary representation.
//
// To calculate those, we need the `number_of_columns` constexpr function that we include
// from "number_of_possible_columns.h", and two additional constexpr functions
// `power` and `num_digits_required`.

constexpr uint64_t power(unsigned a, unsigned b)
{
    return (b == 0) ? 1 : a * power(a, b - 1);
}

constexpr unsigned number_of_digits_required(unsigned base, uint64_t count)
{
    // Find the smallest value 'digits' such that power(base, digits) >= count.

    return (count <= 1) ? 0 : 1 + number_of_digits_required(base, (count + base - 1) / base);
}

const unsigned NUMBER_OF_POSSIBLE_COLUMNS = number_of_possible_columns(CONNECT_Q, V_SIZE);
const uint64_t NUMBER_OF_BOARDS_IN_COLUMN_REPRESENTATION = power(NUMBER_OF_POSSIBLE_COLUMNS, H_SIZE);

const unsigned NUM_BASE62_BOARD_DIGITS  = number_of_digits_required( 62, NUMBER_OF_BOARDS_IN_COLUMN_REPRESENTATION);
const unsigned NUM_BASE256_BOARD_DIGITS = number_of_digits_required(256, NUMBER_OF_BOARDS_IN_COLUMN_REPRESENTATION);

#endif // DERIVED_CONSTANTS_H
