
///////////////////
// permutation.h //
///////////////////

#ifndef PERMUTATION_H
#define PERMUTATION_H

#include "board.h"

class Permutation
{
    public:

        Board apply(const Board & board) const;

        Permutation apply(const Permutation & permutation) const;

        static Permutation identity();

        static Permutation mirror_horizontally();

        static Permutation mirror_vertically();

        static Permutation transpose();

    private: // Instance member variables.

        int entries[V_SIZE * H_SIZE];

    // Friends.

    friend bool operator <  (const Permutation & lhs, const Permutation & rhs);
    friend bool operator != (const Permutation & lhs, const Permutation & rhs);
};

#endif // PERMUTATION_H
