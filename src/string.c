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