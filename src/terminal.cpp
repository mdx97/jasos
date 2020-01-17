#include "terminal.h"

Terminal::Terminal(volatile char *video_pointer, int w, int h)
{
    video_pointer_origin = video_pointer;
    video_pointer_current = video_pointer;
    width = w;
    height = h;
}

void Terminal::clear()
{
    reset_pointer();
    for (int i = 0; i < (width * height); i++)
        putchar(' ');
    reset_pointer();
}

void Terminal::print(const char *string)
{
    int i = 0;
    char c = string[0];

    while (c != '\0') {
        switch (c) {
            case '\n': {
                int offset = ((int)video_pointer_current - (int)video_pointer_origin) % (width * 2);
                video_pointer_current += ((width * 2) - offset);
                break;
            }

            case '\t':
                video_pointer_current += 8;
                break;

            default:
                putchar(c);
        }

        i++;
        c = string[i];
    }
}

void Terminal::putchar(char c)
{
    *video_pointer_current++ = c;
    *video_pointer_current++ = 0x0F;
}

void Terminal::reset_pointer()
{
    video_pointer_current = video_pointer_origin;
}