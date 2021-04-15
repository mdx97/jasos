#include "vga.h"

void syscall_put(int x, int y, char c)
{
    vga_put(x, y, c);
}

void syscall_write(int x, int y, char* string)
{
    vga_write(x, y, string);
}

char* syscall_version()
{
    return "0.0.1";
}