
////////////////
// outcome.cc //
////////////////

#include <stdexcept>

#include "base62.h"
#include "outcome.h"

using namespace std;

const char A_WINS_CHAR        = 'A';
const char B_WINS_CHAR        = 'B';
const char DRAW_CHAR          = '-';
const char INDETERMINATE_CHAR = '?';

istream & operator >> (istream & in, Outcome & outcome)
{
    if (in)
    {
        char outcome_char;

        in >> outcome_char;

        switch (outcome_char)
        {
            case A_WINS_CHAR        : outcome = Outcome::A_WINS; break;
            case B_WINS_CHAR        : outcome = Outcome::B_WINS; break;
            case DRAW_CHAR          : outcome = Outcome::DRAW; break;
            case INDETERMINATE_CHAR : outcome = Outcome::INDETERMINATE; break;
            default : throw runtime_error("Outcome from input stream: bad character.");
        }
        score.ply = base62_string_to_uint64(ply_string);
    }
    return in;
}

ostream & operator << (ostream & out, const Outcome & outcome)
{
    switch (score.outcome)
    {
        case Outcome::A_WINS        : out << A_WINS_CHAR; break;
        case Outcome::B_WINS        : out << B_WINS_CHAR; break;
        case Outcome::DRAW          : out << DRAW_CHAR; break;
        case Outcome::INDETERMINATE : out << INDETERMINATE_CHAR; break;
    }

    return out;
}
