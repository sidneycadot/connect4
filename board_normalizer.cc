
/////////////////////////
// board_normalizer.cc //
/////////////////////////

#include <set>

#include "board_normalizer.h"

using namespace std;

BoardNormalizer::BoardNormalizer()
{
    set<Permutation> permutations_found;
    vector<Permutation> to_be_processed;

    // For connect-4, we only use the `mirror_horizontally` permutation.
    
    vector<Permutation> basic_permutations;
    basic_permutations.push_back(Permutation::mirror_horizontally());

    //basic_permutations.push_back(Permutation::mirror_vertically());
    //
    //if (H_SIZE == V_SIZE)
    //{
    //    basic_permutations.push_back(Permutation::transpose());
    //}

    const Permutation identity_permutation = Permutation::identity();

    to_be_processed.push_back(identity_permutation);

    while (!to_be_processed.empty())
    {
        const Permutation permutation = to_be_processed.back();
        to_be_processed.pop_back();

        if (permutations_found.find(permutation) == permutations_found.end())
        {
            // The permutation was not previously processed; process it now.
            permutations_found.insert(permutation);
            for (const Permutation & basic_permutation : basic_permutations)
            {
                to_be_processed.push_back(basic_permutation.apply(permutation));
            }
        }
    }

    // Copy all permutations found, except the identity permutation,
    // into the `non_identity_permutations` vector.

    for (const Permutation & permutation_found: permutations_found)
    {
        if (permutation_found != identity_permutation)
        {
            non_identity_permutations.push_back(permutation_found);
        }
    }
}

Board BoardNormalizer::normalize(const Board & board)
{
    Board normalized_board(board);

    for (const Permutation & permutation: non_identity_permutations)
    {
        const Board permuted_board = permutation.apply(board);
        if (permuted_board < normalized_board)
        {
            normalized_board = permuted_board;
        }
    }
    return normalized_board;
}
