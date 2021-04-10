#include "shell.h"
#include "kernel.h"
#include "string.h"
#include "utility.h"

#define HISTORY_MAX 20
#define INPUT_BUFFER_SIZE 256

const int SHELL_HEIGHT = 25;
const int SHELL_WIDTH = 80;
const int SHELL_LINE_LENGTH = 2 * SHELL_WIDTH;
const int VIDEO_MEMORY_START = 0xB8000;

const char* SHELL_INPUT_INDICATOR = "> ";

// TODO: Should probably use other keys for these, but our keyboard driver is
// limited at the moment.
const char SHELL_SCROLL_DOWN_KEY = '-';
const char SHELL_SCROLL_UP_KEY = '=';

volatile char* video_pointer;

typedef struct t_input_buffer {
    char data[INPUT_BUFFER_SIZE];
    char hold[INPUT_BUFFER_SIZE];
    int ptr;
    int hold_size;
} InputBuffer;

typedef struct t_history {
    char entries[HISTORY_MAX][INPUT_BUFFER_SIZE];
    int ptr;
    int count;
    int scroll;
} History;

InputBuffer buffer;
History history;

// Clears the input buffer.
void clear_buffer()
{
    buffer.ptr = 0;
    for (int i = 0; i < INPUT_BUFFER_SIZE; i++) {
        buffer.data[i] = '\0';
    }
}

// Scrolls the contents of the shell down by one line.
void scroll()
{
    int full_size = SHELL_LINE_LENGTH * SHELL_HEIGHT;
    int shift_size = full_size - SHELL_LINE_LENGTH;
    volatile char* pointer = (volatile char*)VIDEO_MEMORY_START;

    for (int i = 0; i < full_size; i += 2) {
        pointer[i] = pointer[i + (SHELL_LINE_LENGTH)];
    }
    for (int i = shift_size; i < shift_size; i += 2) {
        pointer[i] = ' ';
    }
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
            if (video_pointer > (volatile char*)VIDEO_MEMORY_START) {
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
    video_pointer = (volatile char*)VIDEO_MEMORY_START;
    buffer.data[0] = '\0';
    buffer.hold[0] = '\0';
    buffer.ptr = 0;
    buffer.hold_size = 0;
    history.ptr = 0;
    history.scroll = 0;
    shell_clear();
}

// Clears the shell and sets the pointer back to the upper left corner of the
// terminal.
void shell_clear()
{
    video_pointer = (volatile char*)VIDEO_MEMORY_START;
    for (int i = 0; i < (SHELL_WIDTH * SHELL_HEIGHT); i++) {
        putchar(' ');
    }
    video_pointer = (volatile char*)VIDEO_MEMORY_START;
}

// Sets the current input to the correct history value based off of the scroll
// value.
void load_history_entry()
{
    for (int i = 0; i < buffer.ptr * 2; i += 2) {
        *video_pointer = ' ';
        video_pointer -= 2;
    }

    *video_pointer = ' ';
    buffer.ptr = 0;
    buffer.data[0] = '\0';

    if (history.scroll != 0) {
        // TODO: Prefer modulo, but negatives seem to be overflowing.
        int index = history.scroll > history.ptr
            ? history.ptr + (HISTORY_MAX - history.scroll)
            : history.ptr - history.scroll;

        char* pointer = (char*)&history.entries[index];
        int size = string_length(pointer);
        memory_copy(pointer, buffer.data, size);
        buffer.data[size] = '\0';
        buffer.ptr = size;
        shell_write(pointer);
    } else {
        memory_copy(buffer.hold, buffer.data, buffer.hold_size);
        buffer.data[buffer.hold_size] = '\0';
        buffer.ptr = buffer.hold_size;
        shell_write(buffer.data);
    }
}

// Sends a character to be processed by the shell.
void shell_input(char c)
{
    if (c == '\b') {
        if (buffer.ptr == 0) {
            return;
        }
        buffer.data[--buffer.ptr] = '\0';
        putchar('\b');

    } else if (c == '\n') {
        putchar('\n');
        system((const char*)buffer.data);

        if (string_length(buffer.data) > 0) {
            memory_copy(buffer.data, &history.entries[history.ptr], buffer.ptr);
            history.entries[history.ptr][buffer.ptr] = '\0';

            history.scroll = 0;
            history.ptr++;
            history.ptr %= HISTORY_MAX;

            if (history.count < HISTORY_MAX) {
                history.count++;
            }
        }

        clear_buffer();
        shell_write(SHELL_INPUT_INDICATOR);

    } else if (c == SHELL_SCROLL_DOWN_KEY) {
        if (history.scroll == 0 && !string_equal(buffer.data, buffer.hold)) {
            memory_copy(buffer.data, buffer.hold, buffer.ptr);
            buffer.hold_size = buffer.ptr;
            buffer.hold[buffer.ptr] = '\0';
        }

        if (history.scroll != history.count) {
            history.scroll++;
        }

        load_history_entry();

    } else if (c == SHELL_SCROLL_UP_KEY) {
        if (history.scroll != 0) {
            history.scroll--;
        }
        load_history_entry();

    } else {
        if (buffer.ptr == INPUT_BUFFER_SIZE - 1) {
            return;
        }
        buffer.data[buffer.ptr] = c;
        buffer.data[++buffer.ptr] = '\0';
        putchar(c);
    }
}

// Prints a string to the shell.
void shell_write(const char* string)
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
void shell_writeline(const char* string)
{
    assert(string, NULL_PARAMETER_ERROR(shell_writeline, string));
    shell_write(string);
    shell_write("\n");
}
