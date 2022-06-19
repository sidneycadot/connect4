
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

    uint8_t to_uint8() const;
    static Score from_uint8(uint8_t score_octet);

    Outcome  outcome; // Game-theoretical result.
    unsigned ply;     // Plies until the result is final, assuming optimal play on both sides. 0 if INDETERMINATE.
};

std::istream & operator >> (std::istream & in, Score & score);
std::ostream & operator << (std::ostream & out, const Score & score);

#endif // SCORE_H
