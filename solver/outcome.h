
///////////////
// outcome.h //
///////////////

#ifndef OUTCOME_H
#define OUTCOME_H

#include <istream>
#include <ostream>

// An Outcome represents the game-theoretical outcome of a game position,
// assuming optimal play by both sides.
//
// The Outcome::INDETERMINATE value is used when the outcome cannot (yet) be determined.

enum class Outcome {
    A_WINS,
    B_WINS,
    DRAW,
    INDETERMINATE
};

std::istream & operator >> (std::istream & in, Outcome & outcome);
std::ostream & operator << (std::ostream & out, const Outcome & outcome);

#endif // OUTCOME_H
