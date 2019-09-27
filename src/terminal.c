#include "terminal.h"

#define VIDEO_MAX 2000 // 80x25 is our maximum size at the moment.

volatile char *video_pointer;

void reset_pointer()
{
    video_pointer = (volatile char*)0xB8000;
}

void terminal_init()
{
    reset_pointer();
    terminal_clear();
}

void terminal_clear()
{
    reset_pointer();
    for (int i = 0; i < VIDEO_MAX; i++)
        terminal_putc(' ');
    reset_pointer();
}

void terminal_putc(char c)
{
    *video_pointer++ = c;
    *video_pointer++ = 0x0F;
}
