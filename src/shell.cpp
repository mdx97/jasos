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

// Clears the shell and sets the pointer back to the upper left corner of the terminal.
void Shell::clear()
{
    reset_pointer();
    for (int i = 0; i < (width * height); i++)
        putchar(' ');
    reset_pointer();
}

// Sends a character to be processed by the shell.
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

// Prints a string to the shell.
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

// Prints the input indicator for the shell.
void Shell::ready_input()
{
    output(">>> ");
}

// Clears the input buffer.
void Shell::clear_buffer()
{
    buffer_ptr = 0;
    for (int i = 0; i < 256; i++)
        buffer[i] = '\0';
}

// Prints a single character to the shell.
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

// Sets the video pointer back to it's original position.
void Shell::reset_pointer()
{
    video_pointer_current = video_pointer_origin;
}

// Scrolls the contents of the shell down by one line.
void Shell::scroll()
{
    // TODO: Clean this function up.
    int end = (2 * width * height) - (2 * width);
    for (int i = 0; i < end; i += 2)
        video_pointer_origin[i] = video_pointer_origin[i + (width * 2)];
    for (int i = end + 2; i < (2 * width * height); i += 2)
        video_pointer_origin[i] = ' ';
}
