// TODO: Need to do loads of bounds testing on whether my calculations for display size are correct and won't result in writing to bad memory.
#include "vga.h"

const int DISPLAY_HEIGHT = 25;
const int DISPLAY_WIDTH = 80;
const int VIDEO_MEMORY_OFFSET = 0xB8000;

volatile char* to_pointer(int x, int y)
{
    return (volatile char*)(VIDEO_MEMORY_OFFSET + (y * DISPLAY_WIDTH * 2) + (x * 2));
}

void put(volatile char* pointer, char c)
{
    *pointer++ = c;
    *pointer++ = 0x0F;
}

void vga_init()
{
    volatile char* pointer = to_pointer(0, 0);
    for (int i = 0; i < DISPLAY_HEIGHT * DISPLAY_WIDTH * 2; i++) {
        *pointer = 0;
        pointer++;
    }
}

void vga_put(int x, int y, char c)
{
    put(to_pointer(x, y), c);
}

// TODO: Handle invalid values for x / y here or higher up?
void vga_write(int x, int y, char* string)
{
    volatile char* pointer = to_pointer(x, y);
    char* char_ptr = &string[0];
    while (*char_ptr != '\0') {
        put(pointer, *char_ptr);
        char_ptr++;
        pointer += 2;
    }
}