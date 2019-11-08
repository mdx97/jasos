#include "bits.h"

// Returns the n least significant bits of a number.
uint32_t lsb(uint32_t number, uint8_t n)
{
    return number & ((1 << n) - 1);
}
