
//////////////
// score.cc //
//////////////

#include "score.h"

#include <stdexcept>

using namespace std;

const char A_WINS_CHAR        = 'A';
const char B_WINS_CHAR        = 'B';
const char DRAW_CHAR          = '-';
const char INDETERMINATE_CHAR = '?';

istream & operator >> (istream & in, Score & score)
{
    if (in)
    {
        char c;
        in >> c;

        switch (c)
        {
            case A_WINS_CHAR        : score = Score::A_WINS; break;
            case B_WINS_CHAR        : score = Score::B_WINS; break;
            case DRAW_CHAR          : score = Score::DRAW; break;
            case INDETERMINATE_CHAR : score = Score::INDETERMINATE; break;
            default : throw runtime_error("bad character for Score from input stream.");
        }
    }
    return in;
}

ostream & operator << (ostream & out, const Score & score)
{
    switch (score)
    {
        case Score::A_WINS        : out << A_WINS_CHAR; break;
        case Score::B_WINS        : out << B_WINS_CHAR; break;
        case Score::DRAW          : out << DRAW_CHAR; break;
        case Score::INDETERMINATE : out << INDETERMINATE_CHAR; break;
        default : throw runtime_error("bad Score for output stream.");
    }
    return out;
}
