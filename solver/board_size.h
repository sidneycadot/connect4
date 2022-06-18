
//////////////////
// board_size.h //
//////////////////

#ifndef BOARD_SIZE_H
#define BOARD_SIZE_H

#include "number_of_possible_columns.h"

// These are the main constants defines the board size and the connect rule.

const int H_SIZE = 5; // Horizontal board size.
const int V_SIZE = 4; // Vertical board size.
const int Q      = 4; // The number of connected horizontal/diagonal/vertical chips required to win.

// Apart from these, we need to define some derived constants that define the storage size of a Board
// when represented in the base-62 ASCII representation and the base-256 binary representation.
//
// To calculate those, we need two constexpr functions that enable the compiler to calculate those
// values at compile-time.

constexpr uint64_t power(unsigned a, unsigned b)
{
    return (b == 0) ? 1 : a * power(a, b - 1);
}

constexpr unsigned num_digits_required(unsigned base, uint64_t value)
{
	// Find the smallest value 'digits' such that power(base, digits) >= value.

    return (value <= 1) ? 0 : 1 + num_digits_required(base, (value + base - 1) / base);
}

const unsigned NUMBER_OF_POSSIBLE_COLUMNS = number_of_possible_columns(Q, V_SIZE);
const uint64_t NUMBER_OF_BOARDS_IN_COLUMN_REPRESENTATION = power(NUMBER_OF_POSSIBLE_COLUMNS, H_SIZE);

const unsigned NUM_BASE62_BOARD_DIGITS  = num_digits_required( 62, NUMBER_OF_BOARDS_IN_COLUMN_REPRESENTATION);
const unsigned NUM_BASE256_BOARD_DIGITS = num_digits_required(256, NUMBER_OF_BOARDS_IN_COLUMN_REPRESENTATION);

const unsigned NUM_BASE62_SCOREPLY_DIGITS = 1; // Assume that winply can always be represented as a 1-character base62 string.

#endif // BOARD_SIZE_H
