
//////////////
// base62.h //
//////////////

#ifndef BASE62_H
#define BASE62_H

#include <cstdint>
#include <string>

// Encode and decode 64-bit unsigned integers as base-62 strings.
// Base-62 strings use the characters 0..9, A..Z, and a..z as digits.

char to_base62_digit(unsigned n);

unsigned from_base62_digit(char c);

std::string uint64_to_base62_string(std::uint64_t n, unsigned num_digits);

uint64_t base62_string_to_uint64(const std::string & s);

#endif // BASE62_H
