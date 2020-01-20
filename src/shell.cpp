#include "kernel.h"
#include "shell.h"

Shell::Shell(volatile char *video_pointer, int w, int h)
{
    video_pointer_origin = video_pointer;
    video_pointer_current = video_pointer;
    width = w;
    height = h;
    buffer_ptr = 0;
}

void Shell::clear()
{
    reset_pointer();
    for (int i = 0; i < (width * height); i++)
        putchar(' ');
    reset_pointer();
}

void Shell::input(char c)
{
    if (c == '\b') {
        if (buffer_ptr == 0) return;
        buffer[buffer_ptr--] = '\0';
        putchar('\b');
    } else if (c == '\n') {
        putchar('\n');
        system((const char *)buffer);
        clear_buffer();
        ready_input();
    } else {
        buffer[buffer_ptr] = c;
        buffer_ptr++;
        putchar(c);
    }
    
}

void Shell::output(const char *string)
{
    int i = 0;
    char c = string[0];

    while (c != '\0') {
        putchar(c);
        i++;
        c = string[i];
    }
}

void Shell::ready_input()
{
    output(">>> ");
}

void Shell::clear_buffer()
{
    buffer_ptr = 0;
    for (int i = 0; i < 256; i++)
        buffer[i] = '\0';
}

void Shell::putchar(char c)
{
    switch (c) {
        case '\b': {
            if (video_pointer_current > video_pointer_origin) {
                video_pointer_current -= 2;
                *video_pointer_current = ' ';
            }
            break;
        }

        case '\n': {
            int offset = ((int)video_pointer_current - (int)video_pointer_origin) % (width * 2);
            video_pointer_current += ((width * 2) - offset);
            if ((int)video_pointer_current >= (int)video_pointer_origin + (2 * width * height)) {
                scroll();
                video_pointer_current -= (width * 2);
            }
            break;
        }

        case '\t': {
            video_pointer_current += 8;
            break;
        }
        
        default: {
            *video_pointer_current++ = c;
            *video_pointer_current++ = 0x0F;
            break;
        }
    }
}

void Shell::reset_pointer()
{
    video_pointer_current = video_pointer_origin;
}

void Shell::scroll()
{
    int end = (2 * width * height) - (2 * width);
    for (int i = 0; i < end; i += 2)
        video_pointer_origin[i] = video_pointer_origin[i + (width * 2)];
    for (int i = end + 2; i < (2 * width * height); i += 2)
        video_pointer_origin[i] = ' ';
}
