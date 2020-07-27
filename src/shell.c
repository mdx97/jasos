#include "kernel.h"
#include "shell.h"
#include "string.h"
#include "utility.h"

#define HISTORY_MAX 20
#define SHELL_INPUT_BUFFER_SIZE 256

const int SHELL_HEIGHT = 25;
const int SHELL_WIDTH = 80;
const int SHELL_LINE_LENGTH = 2 * SHELL_WIDTH;
const int VIDEO_MEMORY_START = 0xB8000;

const char *SHELL_INPUT_INDICATOR = "> ";

// TODO: Should probably use other keys for these, but our keyboard driver is limited at the moment.
const char SHELL_SCROLL_DOWN_KEY = '-';
const char SHELL_SCROLL_UP_KEY = '=';

volatile char *video_pointer;

char buffer[SHELL_INPUT_BUFFER_SIZE];
int buffer_ptr;

char history[HISTORY_MAX][SHELL_INPUT_BUFFER_SIZE];
int history_ptr;
int history_count;
int history_scroll;

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
        pointer[i] = pointer[i + (SHELL_LINE_LENGTH)];

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
    history_ptr = 0;
    history_scroll = 0;
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

// Sets the current input to the correct history value based off of the scroll value.
void load_history_entry()
{
    for (int i = 0; i < buffer_ptr * 2; i += 2) {
        *video_pointer = ' ';
        video_pointer -= 2;
    }
    
    *video_pointer = ' ';
    buffer_ptr = 0;
    buffer[0] = '\0';

    if (history_scroll != 0) {
        // TODO: Prefer modulo, but negatives seem to be overflowing.
        int index = history_scroll > history_ptr ?
            history_ptr + (HISTORY_MAX - history_scroll) :
            history_ptr - history_scroll;
        
        char *pointer = (char *)&history[index];
        int size = string_length(pointer);
        memory_copy(pointer, buffer, size);
        buffer[size] = '\0';
        buffer_ptr = size;
        shell_write(pointer);
    }
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

        memory_copy(buffer, &history[history_ptr], buffer_ptr);
        history[history_ptr][buffer_ptr] = '\0';

        history_scroll = 0;
        history_ptr++;
        history_ptr %= HISTORY_MAX;
        
        if (history_count < HISTORY_MAX)
            history_count++;

        clear_buffer();
        shell_write(SHELL_INPUT_INDICATOR);

    } else if (c == SHELL_SCROLL_DOWN_KEY) {
        if (history_scroll != history_count)
            history_scroll++;
        load_history_entry();

    } else if (c == SHELL_SCROLL_UP_KEY) {
        if (history_scroll != 0)
            history_scroll--;
        load_history_entry();

    } else {
        if (buffer_ptr == SHELL_INPUT_BUFFER_SIZE - 1) 
            return;
        buffer[buffer_ptr] = c;
        buffer[++buffer_ptr] = '\0';
        putchar(c);
    }
    
}

// Prints a string to the shell.
void shell_write(const char *string)
{
    assert(string, NULL_PARAMETER_ERROR(shell_write, string));
    
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
    assert(string, NULL_PARAMETER_ERROR(shell_writeline, string));
    shell_write(string);
    shell_write("\n");
}
