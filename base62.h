
//////////////
// base62.h //
//////////////

#ifndef BASE62_H
#define BASE62_H

#include <cstdint>
#include <string>

char to_base62_digit(unsigned n);

unsigned from_base62_digit(char c);

std::string uint64_to_base62_string(std::uint64_t n, unsigned num_digits);

uint64_t base62_string_to_uint64(const std::string & s);

#endif
