
////////////////////
// permutation.cc //
////////////////////

#include "permutation.h"

#include <stdexcept>

using namespace std;

Board Permutation::apply(const Board & board) const
{
    Board permuted;
    for (int i = 0; i < V_SIZE * H_SIZE; ++i)
    {
        permuted.entries[i] = board.entries[entries[i]];
    }
    return permuted;
}

Permutation Permutation::apply(const Permutation & permutation) const
{
    Permutation permuted;
    for (int i = 0; i < V_SIZE * H_SIZE; ++i)
    {
        permuted.entries[i] = permutation.entries[entries[i]];
    }
    return permuted;
}

// static method
Permutation Permutation::identity()
{
    Permutation permuted;

    for (int i = 0; i < V_SIZE * H_SIZE; ++i)
    {
        permuted.entries[i] = i;
    }
    return permuted;
}

// static method
Permutation Permutation::mirror_horizontally()
{
    Permutation permuted;

    for (int i = 0; i < V_SIZE * H_SIZE; ++i)
    {
        int h = i % H_SIZE;
        int v = i / H_SIZE;

        int hh = (H_SIZE - 1) - h;
        int vv = v;
        int ii = vv * H_SIZE + hh;

        permuted.entries[i] = ii;
    }

    return permuted;
}

// static method
Permutation Permutation::mirror_vertically()
{
    Permutation permuted;

    for (int i = 0; i < V_SIZE * H_SIZE; ++i)
    {
        int h = i % H_SIZE;
        int v = i / H_SIZE;

        int hh = h;
        int vv = (V_SIZE - 1) - v;
        int ii = vv * H_SIZE + hh;

        permuted.entries[i] = ii;
    }
    return permuted;
}

// static method
Permutation Permutation::transpose()
{
    if (H_SIZE != V_SIZE)
    {
        throw runtime_error("Permutation::transpose: bad transpose");
    }

    Permutation permuted;

    for (int i = 0; i < V_SIZE * H_SIZE; ++i)
    {
        int h = i % H_SIZE;
        int v = i / H_SIZE;

        int hh = v;
        int vv = h;
        int ii = vv * H_SIZE + hh;

        permuted.entries[i] = ii;
    }

    return permuted;
}

bool operator < (const Permutation & lhs, const Permutation & rhs)
{
    for (int i = 0; i < V_SIZE * H_SIZE; ++i)
    {
        if (lhs.entries[i] < rhs.entries[i])
        {
            return true;
        }
        if (lhs.entries[i] > rhs.entries[i])
        {
            return false;
        }
    }
    // Boards are identical, so (lhs < rhs) is false.
    return false;
}

bool operator != (const Permutation & lhs, const Permutation & rhs)
{
    for (int i = 0; i < V_SIZE * H_SIZE; ++i)
    {
        if (lhs.entries[i] != rhs.entries[i])
        {
            return true;
        }
    }
    return false;
}