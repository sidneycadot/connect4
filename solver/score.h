
/////////////
// score.h //
/////////////

#ifndef SCORE_H
#define SCORE_H

#include <istream>
#include <ostream>
#include <cstdint>

#include "outcome.h"

// A Score is a game-theoretical Outcome (A_WINS / B_WINS / DRAW / INDETERMINATE) plus a
// number of optimal moves to be made before the game ends in that outcome.

struct Score {

    Score() = default;

    Score(Outcome outcome, unsigned ply) : outcome(outcome), ply(ply)
    {
        // Empty body.
    }

    // Encode a Score as an 8-bit unsigned integer.
    uint8_t to_uint8() const;

    // Decode a Score from an 8-bit unsigned integer.
    static Score from_uint8(uint8_t score_octet);

    // Game-theoretical outcome for this position, assuming optimal play by both sides.
    Outcome outcome;

    // Plies until the result is final (i.e., the game ends with the outcome), assuming optimal play by both sides.
    // This value is meaningless for Outcome::INDETERMINATE; in that case, we store 0 here.
    unsigned ply;
};

std::istream & operator >> (std::istream & in, Score & score);
std::ostream & operator << (std::ostream & out, const Score & score);

#endif // SCORE_H
