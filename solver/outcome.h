
///////////////
// outcome.h //
///////////////

#ifndef OUTCOME_H
#define OUTCOME_H

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

std::istream & operator >> (std::istream & in, Outcome & outcome);
std::ostream & operator << (std::ostream & out, const Outcome & outcome);

#endif // OUTCOME_H
