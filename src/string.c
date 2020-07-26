#include "string.h"

// Returns if two strings are equal.
bool string_equal(const char *string1, const char *string2)
{
    int i = 0;
    while (string1[i] != '\0' && string2[i] != '\0') {
        if (string1[i] != string2[i]) return false;
        i++;
    }

    return string1[i] == '\0' && string2[i] == '\0';
}

// Returns the length of a string.
int string_length(const char *string)
{
    int i = 0;
    while (true) {
        if (string[i] == '\0')
            break;
        i++;
    }

    return i;
}
