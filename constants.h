
/////////////////
// constants.h //
/////////////////

#ifndef CONSTANTS_H
#define CONSTANTS_H

// These are the counts of possible legal columns in connect-4,
// when the board has a certain height V_SIZE:
//
// V_SIZE  column_count
//   0         1    
//   1         3
//   2         7
//   3        15
//   4        31
//   5        59
//   6       111
//   7       207
//   8       383
//   9       707
//  10      1303

const int V_SIZE = 5; // Vertical board size.
const int H_SIZE = 6; // Horizontal board size.
const int Q      = 4; // Number of consecutive horizontal/diagonal/vertical cells required to win.

// The number of base-62 digits needed to represent a single board state:
//
//   ceiling(log(column_count**H_SIZE) / log(62))

const unsigned NUM_BASE62_DIGITS_v5h6q4 = 6; // 5x6 board, connect-4.
const unsigned NUM_BASE62_DIGITS_v6h7q4 = 8; // 6x7 board, connect-4.

const unsigned NUM_BASE62_DIGITS = NUM_BASE62_DIGITS_v6h7q4;

// The number of base-256 digits needed to represent a single board state:
//
//   ceiling(log(column_count**H_SIZE) / log(256))

const unsigned NUM_OCTETS_v5h6q4 = 5; // 5x6 board, connect-4
const unsigned NUM_OCTETS_v6h7q4 = 6; // 6x7 board, connect-4

const unsigned NUM_OCTETS = NUM_OCTETS_v6h7q4;

#endif // CONSTANTS_H
