
/////////////
// score.h //
/////////////

#ifndef SCORE_H
#define SCORE_H

#include <istream>
#include <ostream>

// The Outcome represents the game-theoretical outcome of a game position,
// assuming both players play optimally.
//
// The "INDETERMINATE" value is used when the Outcome cannot (yet) be determined.

enum class Outcome {
    A_WINS,
    B_WINS,
    DRAW,
    INDETERMINATE
};

struct Score {

    Score() = default;

    Score(Outcome outcome, unsigned ply) : outcome(outcome), ply(ply)
    {
        // Empty body.
    }

    uint8_t to_uint8() const;

    Outcome  outcome; // Game-theoretical result.
    unsigned ply;     // Plies until the result is final, assuming optimal play on both sides. 0 if INDETERMINATE.
};

std::istream & operator >> (std::istream & in, Score & score);
std::ostream & operator << (std::ostream & out, const Score & score);

#endif // SCORE_H
