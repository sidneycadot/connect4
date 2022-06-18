
/////////////
// score.h //
/////////////

#ifndef SCORE_H
#define SCORE_H

#include <istream>
#include <ostream>

enum class Score : char {
    A_WINS,
    B_WINS,
    DRAW,
    INDETERMINATE
};

std::istream & operator >> (std::istream & in, Score & score);
std::ostream & operator << (std::ostream & out, const Score & score);

#endif // SCORE_H
