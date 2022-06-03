
//////////////
// player.h //
//////////////

#ifndef PLAYER_H
#define PLAYER_H

#include <istream>
#include <ostream>

enum class Player : char {
    EMPTY,
    A,
    B
};

std::istream & operator >> (std::istream & in, Player & player);
std::ostream & operator << (std::ostream & out, const Player & player);

#endif // PLAYER_H
