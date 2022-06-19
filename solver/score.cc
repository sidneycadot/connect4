
//////////////
// score.cc //
//////////////

#include <stdexcept>
#include <iomanip>

#include "score.h"
#include "base62.h"

using namespace std;

const char A_WINS_CHAR        = 'A';
const char B_WINS_CHAR        = 'B';
const char DRAW_CHAR          = '-';
const char INDETERMINATE_CHAR = '?';

const unsigned PLY_DIGITS = 1; // Number of base-62 digits to use for the "ply" field.

istream & operator >> (istream & in, Score & score)
{
    if (in)
    {
        char outcome_char;
        string ply_string;

        in >> outcome_char >> setw(PLY_DIGITS) >> ply_string;

        switch (outcome_char)
        {
            case A_WINS_CHAR        : score.outcome = Outcome::A_WINS; break;
            case B_WINS_CHAR        : score.outcome = Outcome::B_WINS; break;
            case DRAW_CHAR          : score.outcome = Outcome::DRAW; break;
            case INDETERMINATE_CHAR : score.outcome = Outcome::INDETERMINATE; break;
            default : throw runtime_error("score input: bad character for outcome field from input stream.");
        }
        score.ply = base62_string_to_uint64(ply_string);
    }
    return in;
}

ostream & operator << (ostream & out, const Score & score)
{
    switch (score.outcome)
    {
        case Outcome::A_WINS        : out << A_WINS_CHAR; break;
        case Outcome::B_WINS        : out << B_WINS_CHAR; break;
        case Outcome::DRAW          : out << DRAW_CHAR; break;
        case Outcome::INDETERMINATE : out << INDETERMINATE_CHAR; break;
    }

    out << uint64_to_base62_string(score.ply, PLY_DIGITS);

    return out;
}

uint8_t Score::to_uint8() const
{
    uint8_t outcome_bits = 0;

    switch (outcome)
    {
        case Outcome::A_WINS       : outcome_bits = 0x40; break;
        case Outcome::B_WINS       : outcome_bits = 0x80; break;
        case Outcome::INDETERMINATE: outcome_bits = 0xc0; break;
        case Outcome::DRAW         : outcome_bits = 0x00; break;
    }

    return outcome_bits | ply;
}
