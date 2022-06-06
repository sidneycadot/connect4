
///////////////
// player.cc //
///////////////

#include "player.h"

#include <stdexcept>

using namespace std;

istream & operator >> (istream & in, Player & player)
{
    if (in)
    {
        char c;
        in >> c;

        switch (c)
        {
            case 'A' : player = Player::A   ; break;
            case 'B' : player = Player::B   ; break;
            case '.' : player = Player::NONE; break;
            default : throw runtime_error("bad character for Player from input stream.");
        }
    }
    return in;
}

ostream & operator << (ostream & out, const Player & player)
{
    switch (player)
    {
        case Player::A    : out << 'A'; break;
        case Player::B    : out << 'B'; break;
        case Player::NONE : out << '.'; break;
        default : throw runtime_error("bad Player for output stream.");
    }
    return out;
}
