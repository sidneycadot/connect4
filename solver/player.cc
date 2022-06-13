
///////////////
// player.cc //
///////////////

#include "player.h"

#include <stdexcept>

using namespace std;

const char PLAYER_A_CHAR    = 'A';
const char PLAYER_B_CHAR    = 'B';
const char PLAYER_NONE_CHAR = '.';

istream & operator >> (istream & in, Player & player)
{
    if (in)
    {
        char c;
        in >> c;

        switch (c)
        {
            case PLAYER_A_CHAR    : player = Player::A; break;
            case PLAYER_B_CHAR    : player = Player::B; break;
            case PLAYER_NONE_CHAR : player = Player::NONE; break;
            default : throw runtime_error("bad character for Player from input stream.");
        }
    }
    return in;
}

ostream & operator << (ostream & out, const Player & player)
{
    switch (player)
    {
        case Player::A    : out << PLAYER_A_CHAR; break;
        case Player::B    : out << PLAYER_B_CHAR; break;
        case Player::NONE : out << PLAYER_NONE_CHAR; break;
        default : throw runtime_error("bad Player for output stream.");
    }
    return out;
}
