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

void terminal_print(char *string)
{
    int i = 0;
    char c = string[0];

    while (c != '\0') {
        switch (c) {
            case '\n': {
                int offset = ((int)video_pointer - 0xB8000) % 160;
                video_pointer += (160 - offset);
                break;
            }

            case '\t':
                video_pointer += 8;
                break;

            default:
                terminal_putc(c);
        }

        i++;
        c = string[i];
    }
}

void terminal_putc(char c)
{
    *video_pointer++ = c;
    *video_pointer++ = 0x0F;
}
