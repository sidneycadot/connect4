
//////////////////////////////////
// number_of_possible_columns.h //
//////////////////////////////////

#ifndef NUMBER_OF_POSSIBLE_COLUMNS_H
#define NUMBER_OF_POSSIBLE_COLUMNS_H

// The constexpr function 'number_of_possible_columns(q, n)' calculates the number of possible connect-q columns
// of height up to and including n.
//
// These are the number_of_possible_columns(q, n), with the winning sequence q ranging from 0 to 10
// and the board height n ranging from 0 to 20:
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
//
// The calculation is implemented by evaluating the following recursive relations:
//
//    number_of_possible_columns(n, q) = 2 ** n - 1                                                       ;;  if n <= q   (Eq.1)
//    number_of_possible_columns(n, q) = sum(k = [1 .. q-1] number_of_possible_columns(n - k)) + (q + 2)  ;;  if n >  q   (Eq.2)
//
// TODO: Add the justification of Eq.1 and Eq.2.

constexpr unsigned number_of_possible_columns(unsigned q, unsigned n);

constexpr unsigned _number_of_possible_columns_sum(unsigned q, unsigned a, unsigned b)
{
    return number_of_possible_columns(q, a) + ((a == b) ? 0 : _number_of_possible_columns_sum(q, a + 1, b));
}

constexpr unsigned number_of_possible_columns(unsigned q, unsigned n)
{
    return (n <= q) ? (2 << n) - 1 : _number_of_possible_columns_sum(q, n - q + 1, n - 1) + q + 2;
}

#endif // NUMBER_OF_POSSIBLE_COLUMNS_H
