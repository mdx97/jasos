#include "utility.h"
#include "kernel.h"
#include "shell.h"

const int DIGIT_ASCII_OFFSET = 48;
const int UINT8_STRING_LENGTH = 9;
const int UINT32_STRING_LENGTH = 11;
const int UINT32_BITSTRING_LENGTH = 35;

// Copys a section of memory from the source to the target.
void memory_copy(void* source, void* target, int count)
{
    assert(source, NULL_PARAMETER_ERROR(memory_copy, source));
    assert(target, NULL_PARAMETER_ERROR(memory_copy, target));

    for (int i = 0; i < count; i++) {
        *(uint8_t*)target = *(uint8_t*)source;
        source++;
        target++;
    }
}

// Fills a section of memory with the given value.
void memory_fill(void* pointer, uint8_t value, int count)
{
    assert(pointer, NULL_PARAMETER_ERROR(memory_fill, pointer));
    for (int i = 0; i < count; i++) {
        *(uint8_t*)pointer = value;
        pointer++;
    }
}

// TODO: Probably should do some limit checking so we don't get overflow, but it's not too dangerous since we are using uints.
uint32_t power(uint32_t base, uint32_t exponent)
{
    if (exponent == 0) {
        return 1;
    }

    uint32_t result = base;
    for (uint32_t i = 1; i < exponent; i++) {
        result *= base;
    }

    return result;
}

// Simply converts an integer into it's string representation.
// note: the string argument should be allocated with UINT32_STRING_LENGTH bytes.
void uint_to_string(uint32_t number, char* string)
{
    assert(string, NULL_PARAMETER_ERROR(uint_to_string, string));

    if (number == 0) {
        string[0] = '0';
        string[1] = '\0';
        return;
    }

    char buffer[UINT32_STRING_LENGTH];
    int size = 0;

    while (number > 0) {
        int digit = number % 10;
        buffer[size] = (char)(digit + DIGIT_ASCII_OFFSET);
        number /= 10;
        size++;
    }

    int index = size - 1;
    while (index >= 0) {
        string[size - index - 1] = buffer[index];
        index--;
    }

    string[size] = '\0';
}

// Converts an integer into a 32-bit "bit string".
// example: 15 -> "0b00000000000000000000000000001111"
// note: the string argument should be allocated with UINT32_BITSTRING_LENGTH bytes.
void uint_to_bitstring(uint32_t number, char* string)
{
    assert(string, NULL_PARAMETER_ERROR(uint_to_bitstring, string));

    string[0] = '0';
    string[1] = 'b';

    for (int i = 0; i < 32; i++) {
        uint32_t value = power(2, 31 - i);
        if (number >= value) {
            number -= value;
            string[i + 2] = '1';
        } else {
            string[i + 2] = '0';
        }
    }

    string[UINT32_BITSTRING_LENGTH - 1] = '\0';
}

// Helper function for the memory_dump() function to format bitstrings, or potential series of bitstrings.
void memory_dump_bitstring_format(char* source, char* dest, uint8_t bytes)
{
    for (int i = 0; i < bytes * 8; i++) {
        dest[bytes * 8 - i - 1] = source[UINT32_BITSTRING_LENGTH - 2 - i];
    }
    dest[bytes * 8] = '\0';
}

/*
    Prints out memory contents.

    Parameters:
    - address:      The address to begin the dump.
    - bytes_per:    The amount of bytes per dump row. Must be in the range [1, 4].
    - count:        The number of rows to print.
*/
void memory_dump(uint32_t address, uint8_t bytes_per, uint32_t count)
{
    if (bytes_per == 0 || bytes_per > 4) {
        shell_write("Memory dump called with invalid bytes_per argument: ");
        char bytes_per_string[UINT8_STRING_LENGTH];
        uint_to_string(bytes_per, (char*)&bytes_per_string);
        shell_writeline(bytes_per_string);
        return;
    }

    uint8_t* pointer = (uint8_t*)address;

    char temp[UINT8_STRING_LENGTH];
    char string[UINT8_STRING_LENGTH];

    for (uint32_t i = 0; i < count; i++) {
        shell_write("0b");
        for (uint8_t j = 0; j < bytes_per; j++) {
            uint_to_bitstring(*pointer, (char*)&temp);
            memory_dump_bitstring_format((char*)&temp, (char*)&string, bytes_per);
            shell_write(string);
            pointer++;
        }
        shell_write("\n");
    }
}
