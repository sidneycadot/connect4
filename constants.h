
/////////////////
// constants.h //
/////////////////

#ifndef CONSTANTS_H
#define CONSTANTS_H

const int V_SIZE = 6; // Vertical board size.
const int H_SIZE = 7; // Horizontal board size.
const int Q      = 4; // Number of consecutive horizontal/diagonal/vertical cells required to win.

// Note: the numbers below are specific to (V_SIZE, H_SIZE) == (6, 7).

// We need 8 base-62 digits to represent a single board state: ceiling(log(111**7) / log(62))
const unsigned NUM_BASE62_DIGITS = 8;

// We need 6 base-256 digits to encode a single board state: ceiling(log(111**7) / log(256))
const unsigned NUM_OCTETS = 6;

#endif // CONSTANTS_H
