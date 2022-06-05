
///////////////////
// permutation.h //
///////////////////

#ifndef PERMUTATION_H
#define PERMUTATION_H

#include "board.h"

class Permutation
{
    // Permutation change the board in ways that keeps the important structure intact.
    // In connect-4, we only really need the `mirror_vertically` permutation;
    // for other games however, the `mirror_vertically` and `transpose` permutations
    // are defined as well.

    public:

        Board apply(const Board & board) const;

        Permutation apply(const Permutation & permutation) const;

        static Permutation identity();

        static Permutation mirror_horizontally();

        static Permutation mirror_vertically();

        static Permutation transpose();

    private: // Member variables.

        int entries[V_SIZE * H_SIZE];

    // Friends.

        friend bool operator <  (const Permutation & lhs, const Permutation & rhs);
        friend bool operator != (const Permutation & lhs, const Permutation & rhs);
};

#endif // PERMUTATION_H
