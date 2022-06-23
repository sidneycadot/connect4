
//////////////
// score.cc //
//////////////

#include <stdexcept>
#include <iomanip>
#include <string>

#include "score.h"
#include "base62.h"

using namespace std;

constexpr unsigned BASE62_PLY_DIGITS = 1; // Number of base-62 digits to use for the "ply" field.

istream & operator >> (istream & in, Score & score)
{
    if (in)
    {
        string ply_string;

        in >> score.outcome >> setw(BASE62_PLY_DIGITS) >> ply_string;
        score.ply = base62_string_to_uint64(ply_string);
    }
    return in;
}

ostream & operator << (ostream & out, const Score & score)
{
    out << score.outcome << uint64_to_base62_string(score.ply, BASE62_PLY_DIGITS);
    return out;
}

uint8_t Score::to_uint8() const
{
    uint8_t outcome_bits = 0;

    switch (outcome)
    {
        case Outcome::A_WINS       : outcome_bits = 0x40; break;
        case Outcome::B_WINS       : outcome_bits = 0x80; break;
        case Outcome::DRAW         : outcome_bits = 0x00; break;
        case Outcome::INDETERMINATE: outcome_bits = 0xc0; break;
    }

    return outcome_bits | ply;
}

// static method
Score Score::from_uint8(uint8_t score_octet)
{
    Outcome outcome = Outcome::INDETERMINATE;

    switch (score_octet & 0xc0)
    {
        case 0x40: outcome = Outcome::A_WINS; break;
        case 0x80: outcome = Outcome::B_WINS; break;
        case 0x00: outcome = Outcome::DRAW; break;
        case 0xc0: outcome = Outcome::INDETERMINATE; break;
    }

    return Score(outcome, score_octet & 0x3f);
}
