#ifndef UTILITY_H
#define UTILITY_H

#include <stdint.h>

extern const int
    UINT8_STRING_LENGTH,
    UINT32_MAX_DIGITS,
    UINT32_BITSTRING_LENGTH;

void memory_fill(void *pointer, uint8_t value, int count);
void uint_to_string(uint32_t number, char *string);
void uint_to_bitstring(uint32_t number, char *string);
void memory_dump(uint32_t address, uint8_t bytes_per, uint32_t count);

#endif
