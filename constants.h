
/////////////////
// constants.h //
/////////////////

#ifndef CONSTANTS_H
#define CONSTANTS_H

// These are the counts of possible legal columns in connect-4-like games,
// when the board has a certain height V_SIZE (0..20), and the winning sequence has length Q (0..10).
//
//    |         0         1         2         3         4         5         6         7         8         9        10
// ---+--------------------------------------------------------------------------------------------------------------
//  0 |         1         1         1         1         1         1         1         1         1         1         1
//  1 |         1         3         3         3         3         3         3         3         3         3         3
//  2 |         1         3         7         7         7         7         7         7         7         7         7
//  3 |         1         3        11        15        15        15        15        15        15        15        15
//  4 |         1         3        15        27        31        31        31        31        31        31        31
//  5 |         1         3        19        47        59        63        63        63        63        63        63
//  6 |         1         3        23        79       111       123       127       127       127       127       127
//  7 |         1         3        27       131       207       239       251       255       255       255       255
//  8 |         1         3        31       215       383       463       495       507       511       511       511
//  9 |         1         3        35       351       707       895       975      1007      1019      1023      1023
// 10 |         1         3        39       571      1303      1727      1919      1999      2031      2043      2047
// 11 |         1         3        43       927      2399      3331      3775      3967      4047      4079      4091
// 12 |         1         3        47      1503      4415      6423      7423      7871      8063      8143      8175
// 13 |         1         3        51      2435      8123     12383     14595     15615     16063     16255     16335
// 14 |         1         3        55      3943     14943     23871     28695     30975     31999     32447     32639
// 15 |         1         3        59      6383     27487     46015     56415     61443     63743     64767     65215
// 16 |         1         3        63     10331     50559     88699    110911    121879    126975    129279    130303
// 17 |         1         3        67     16719     92995    170975    218047    241759    252931    258047    260351
// 18 |         1         3        71     27055    171047    329567    428671    479551    503831    515071    520191
// 19 |         1         3        75     43779    314607    635263    842747    951231   1003615   1028099   1039359
// 20 |         1         3        79     70839    578655   1224511   1656799   1886847   1999167   2052119   2076671

const int V_SIZE = 6; // Vertical board size.
const int H_SIZE = 7; // Horizontal board size.
const int Q      = 4; // Number of consecutive horizontal/diagonal/vertical cells required to win.

// The number of base-62 digits needed to represent a single board state:
//
//   ceiling(H_SIZE * log(column_count) / log(62))

const unsigned NUM_BASE62_DIGITS_v5h6q4 = 6; // 5x6 board, connect-4.
const unsigned NUM_BASE62_DIGITS_v6h7q4 = 8; // 6x7 board, connect-4.

const unsigned NUM_BASE62_DIGITS = NUM_BASE62_DIGITS_v6h7q4;

// The number of base-256 digits needed to represent a single board state:
//
//   ceiling(H_SIZE * log(column_count) / log(256))

const unsigned NUM_OCTETS_v5h6q4 = 5; // 5x6 board, connect-4
const unsigned NUM_OCTETS_v6h7q4 = 6; // 6x7 board, connect-4

const unsigned NUM_OCTETS = NUM_OCTETS_v6h7q4;

#endif // CONSTANTS_H
