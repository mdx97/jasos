#include "kernel.h"
#include "shell.h"

#define SHELL_INPUT_BUFFER_SIZE 256
#define SHELL_HEIGHT 25
#define SHELL_WIDTH 80
#define SHELL_LINE_LENGTH SHELL_WIDTH * 2
#define VIDEO_MEMORY_START 0xB8000

const char *SHELL_INPUT_INDICATOR = "> ";

volatile char *video_pointer;
char buffer[SHELL_INPUT_BUFFER_SIZE];
int buffer_ptr;

// Clears the input buffer.
void clear_buffer()
{
    buffer_ptr = 0;
    for (int i = 0; i < SHELL_INPUT_BUFFER_SIZE; i++)
        buffer[i] = '\0';
}

// Scrolls the contents of the shell down by one line.
void scroll()
{
    int full_size = SHELL_LINE_LENGTH * SHELL_HEIGHT;
    int shift_size = full_size - SHELL_LINE_LENGTH;

    volatile char *pointer = (volatile char *)VIDEO_MEMORY_START;

    for (int i = 0; i < full_size; i += 2)
        pointer[i] = pointer[i + SHELL_LINE_LENGTH];

    for (int i = shift_size; i < shift_size; i += 2)
        pointer[i] = ' ';
}

// Scrolls the contents of the shell if needed.
void handle_scroll()
{
    if ((int)video_pointer >= VIDEO_MEMORY_START + (SHELL_LINE_LENGTH * SHELL_HEIGHT)) {
        scroll();
        video_pointer -= SHELL_LINE_LENGTH;
    }
}

// Prints a single character to the shell.
void putchar(char c)
{
    switch (c) {
        case '\b': {
            if (video_pointer > (volatile char *)VIDEO_MEMORY_START) {
                video_pointer -= 2;
                *video_pointer = ' ';
            }
            break;
        }

        case '\n': {
            int offset = ((int)video_pointer - VIDEO_MEMORY_START) % SHELL_LINE_LENGTH;
            video_pointer += (SHELL_LINE_LENGTH - offset);
            handle_scroll();
            break;
        }

        case '\t': {
            video_pointer += 8;
            break;
        }
        
        default: {
            handle_scroll();
            *video_pointer++ = c;
            *video_pointer++ = 0x0F;
            break;
        }
    }
}

// Initializes the shell.
void shell_init()
{
    video_pointer = (volatile char *)VIDEO_MEMORY_START;
    buffer_ptr = 0;
    shell_clear();
}

// Clears the shell and sets the pointer back to the upper left corner of the terminal.
void shell_clear()
{
    video_pointer = (volatile char *)VIDEO_MEMORY_START;

    for (int i = 0; i < (SHELL_WIDTH * SHELL_HEIGHT); i++)
        putchar(' ');

    video_pointer = (volatile char *)VIDEO_MEMORY_START;
}

// Sends a character to be processed by the shell.
void shell_input(char c)
{
    if (c == '\b') {
        if (buffer_ptr == 0)
            return;
        buffer[--buffer_ptr] = '\0';
        putchar('\b');

    } else if (c == '\n') {
        putchar('\n');
        system((const char *)buffer);
        clear_buffer();
        shell_write(SHELL_INPUT_INDICATOR);

    } else {
        if (buffer_ptr == SHELL_INPUT_BUFFER_SIZE - 1) 
            return;
        buffer[buffer_ptr] = c;
        buffer_ptr++;
        putchar(c);
    }
    
}

// Prints a string to the shell.
void shell_write(const char *string)
{
    int i = 0;
    char c = string[0];

    while (c != '\0') {
        putchar(c);
        i++;
        c = string[i];
    }
}

// Prints a string to the shell, followed by a newline.
void shell_writeline(const char *string)
{
    shell_write(string);
    shell_write("\n");
}
