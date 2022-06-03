
/////////////////
// constants.h //
/////////////////

#ifndef CONSTANTS_H
#define CONSTANTS_H

const int V_SIZE = 6; // Board vertical board size
const int H_SIZE = 7; // Board horizontal board size
const int Q      = 4; // Number of consecutive horizontal/diagonal/vertical cells required to win.

// We need 8 base-62 characters to encode a single board state: ceiling(log(111**7) / log(62))
const unsigned NUM_BASE62_DIGITS = 8;

#endif // CONSTANTS_H
