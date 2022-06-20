
/////////////
// score.h //
/////////////

#ifndef SCORE_H
#define SCORE_H

#include <istream>
#include <ostream>
#include <cstdint>

#include "outcome.h"

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
    Outcome  outcome;

    // Plies until the result is final (i.e., the game ends with the outcome), assuming optimal play by both sides. 0 if INDETERMINATE.
    unsigned ply;
};

std::istream & operator >> (std::istream & in, Score & score);
std::ostream & operator << (std::ostream & out, const Score & score);

#endif // SCORE_H
