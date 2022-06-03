
////////////////////////
// board_normalizer.h //
////////////////////////

#ifndef BOARD_NORMALIZER_H
#define BOARD_NORMALIZER_H

#include <vector>

#include "board.h"
#include "permutation.h"

class BoardNormalizer
{
    public:

        // Instantiate BoardNormalizer by enumerating all non-identity permutations.
        BoardNormalizer();

        // Normalize a given Board.
        Board normalize(const Board & board);

    private: // Member variables.

        std::vector<Permutation> non_identity_permutations;
};

#endif // BOARD_NORMALIZER_H
