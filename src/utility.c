#include "shell.h"
#include "utility.h"

// TODO: Probably should do some limit checking so we don't get overflow, but it's not too dangerous since we are using uints.
uint32_t power(uint32_t base, uint32_t exponent)
{
    if (exponent == 0) return 1;

    uint32_t result = base;
    for (uint32_t i = 1; i < exponent; i++)
        result *= base;
    return result;
}

void uint_to_string(uint32_t number, char *string)
{
    if (number == 0) {
        string[0] = '0';
        string[1] = '\0';
        return;
    }

    char buffer[11];
    int size = 0;

    while (number > 0) {
        int digit = number % 10;
        buffer[size] = (char)(digit + 48);
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

void uint_to_bitstring(uint32_t number, char *string)
{
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

    string[34] = '\0';
}

void memory_dump_bitstring_format(char *source, char *dest, uint8_t bytes)
{
    for (int i = 0; i < bytes * 8; i++)
        dest[bytes * 8 - i - 1] = source[33 - i];
    dest[bytes * 8] = '\0';
}

void memory_dump(uint32_t address, uint8_t bytes_per, uint32_t count)
{
    if (bytes_per > 4) {
        shell_output("Memory dump called with invalid bytes_per argument: ");
        char bytes_per_string[9];
        uint_to_string(bytes_per, (char *)&bytes_per_string);
        shell_output(bytes_per_string);
        shell_output("\n");
        return;
    }

    uint8_t *pointer = (uint8_t *)address;
    char temp[9];
    char string[9];

    for (uint32_t i = 0; i < count; i++) {
        shell_output("0b");
        for (uint8_t j = 0; j < bytes_per; j++) {
            uint_to_bitstring(*pointer, (char *)&temp);
            memory_dump_bitstring_format((char *)&temp, (char *)&string, bytes_per);
            shell_output(string);
            pointer++;
        }
        shell_output("\n");
    }
}
