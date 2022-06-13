
///////////////
// base62.cc //
///////////////

#include <stdexcept>

#include "base62.h"

using namespace std;

static char to_base62_digit(unsigned n)
{
    if (n < 10)
    {
        return ('0' -  0) + n;
    }
    if (n < 36)
    {
        return ('A' - 10) + n;
    }
    if (n < 62)
    {
        return ('a' - 36) + n;
    }
    throw runtime_error("to_base62_digit: bad base-62 digit");
}

static unsigned from_base62_digit(char c)
{
    if ('0' <= c && c <= '9')
    {
        return (c - '0');
    }
    if ('A' <= c && c <= 'Z')
    {
        return (c - 'A') + 10;
    }
    if ('a' <= c && c <= 'z')
    {
        return (c - 'a') + 36;
    }
    throw runtime_error("from_base62_digit: bad base-62 digit");
}

string uint64_to_base62_string(uint64_t n, unsigned num_digits)
{
    // Encode 'n' as a base-62 number.
    // Digit order is big-endian (i.e., the most significant digit comes first).

    char encoded_number[num_digits + 1];

    for (int i = num_digits - 1; i >= 0; --i)
    {
        encoded_number[i] = to_base62_digit(n % 62);
        n /= 62;
    }

    encoded_number[num_digits] = '\0'; // Terminating NUL character.

    if (n != 0)
    {
        throw runtime_error("uint64_to_base62_string: number too large");
    }

    return string(encoded_number);
}

uint64_t base62_string_to_uint64(const string & s)
{
    // Parse a big-endian base-62 string to an uint64_t.
    // Note: this function does not guard against overflow.
    uint64_t n = 0;
    for (int i = 0; s[i] != '\0'; ++i)
    {
        n = (62 * n) + from_base62_digit(s[i]);
    }
    return n;
}
