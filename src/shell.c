#include "shell.h"
#include "kernel.h"
#include "string.h"
#include "syscalls.h"
#include "utility.h"

// TEMP: Until dynamic memory allocation exists.
#define HISTORY_MAX 20
#define LINES_MAX 256
#define INPUT_BUFFER_SIZE 256

// TODO: Find a way to derive this from kernel?
const int HEIGHT = 25;
const int WIDTH = 80;

const char* INPUT_INDICATOR = "$ ";

// TODO: Should probably use other keys for these, but our keyboard driver is limited at the moment.
const char SCROLL_DOWN_KEY = '-';
const char SCROLL_UP_KEY = '=';

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

typedef struct t_position {
    int x;
    int y;
} Position;

InputBuffer buffer;
History history;
Position cursor_pos;
// char lines[LINES_MAX][WIDTH];
int offset;

// Clears the input buffer.
void clear_buffer()
{
    buffer.ptr = 0;
    for (int i = 0; i < INPUT_BUFFER_SIZE; i++) {
        buffer.data[i] = '\0';
    }
}

// Invokes a shell command.
void run(const char* command)
{
    assert(command, NULL_PARAMETER_ERROR(system, command));
    if (string_equal(command, "clear")) {
        shell_clear();
    } else if (string_equal(command, "help")) {
        shell_output("The kernel supports the following built-in commands:\n- clear: Clears the screen\n- help: How did you get here?\n- system: Prints basic system information");
    } else if (string_equal(command, "system")) {
        shell_output("\n"
                     " JJJJJJJJJ  AAAAAAAAA  SSSSSSSSS  OOOOOOOOO  SSSSSSSSS\n"
                     " JJJJJJJJJ  AAAAAAAAA  SSSSSSSSS  OOOOOOOOO  SSSSSSSSS\n"
                     "    JJJ     AAA   AAA  SSS        OOO   OOO  SSS      \n"
                     "    JJJ     AAA   AAA  SSS        OOO   OOO  SSS      \n"
                     "    JJJ     AAAAAAAAA  SSSSSSSSS  OOO   OOO  SSSSSSSSS\n"
                     "    JJJ     AAA   AAA  SSSSSSSSS  OOO   OOO  SSSSSSSSS\n"
                     "JJJ JJJ     AAA   AAA        SSS  OOO   OOO        SSS\n"
                     "JJJ JJJ     AAA   AAA        SSS  OOO   OOO        SSS\n"
                     "JJJJJJJ     AAA   AAA  SSSSSSSSS  OOOOOOOOO  SSSSSSSSS\n"
                     "JJJJJJJ     AAA   AAA  SSSSSSSSS  OOOOOOOOO  SSSSSSSSS\n\n"
                     "Kernel Version: 0.01\n");
    } else if (!string_equal(command, "")) {
        shell_output("Command not found.");
    }
}

void scroll()
{
    // if (cursor_pos.y > HEIGHT) {
    //     int full_size = SHELL_LINE_LENGTH * SHELL_HEIGHT;
    //     int shift_size = full_size - SHELL_LINE_LENGTH;
    //     volatile char* pointer = (volatile char*)VIDEO_MEMORY_START;

    //     for (int i = 0; i < full_size; i += 2) {
    //         pointer[i] = pointer[i + (SHELL_LINE_LENGTH)];
    //     }
    //     for (int i = shift_size; i < shift_size; i += 2) {
    //         pointer[i] = ' ';
    //     }
    //     cursor_pos.y--;
    // }
}

// Initializes the shell.
void shell_init()
{
    buffer.data[0] = '\0';
    buffer.hold[0] = '\0';
    buffer.ptr = 0;
    buffer.hold_size = 0;
    history.ptr = 0;
    history.scroll = 0;
    syscall_write(0, 0, (char*)INPUT_INDICATOR);
    cursor_pos.x = string_length(INPUT_INDICATOR);
    cursor_pos.y = 0;
    // for (int i = 0; i < LINES_MAX; i++) {
    //     lines[i][0] = '\0';
    // }
    offset = 0;
}

// Clears the shell and sets the pointer back to the upper left corner of the terminal.
void shell_clear()
{
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            syscall_put(i, j, ' ');
        }
    }
    cursor_pos.x = 0;
    cursor_pos.y = 0;
}

// Sets the current input to the correct history value based off of the scroll value.
void load_history_entry()
{
    // for (int i = 0; i < buffer.ptr * 2; i += 2) {
    //     *video_pointer = ' ';
    //     video_pointer -= 2;
    // }

    // *video_pointer = ' ';
    // buffer.ptr = 0;
    // buffer.data[0] = '\0';

    // if (history.scroll != 0) {
    //     // TODO: Prefer modulo, but negatives seem to be overflowing.
    //     int index = history.scroll > history.ptr
    //         ? history.ptr + (HISTORY_MAX - history.scroll)
    //         : history.ptr - history.scroll;

    //     char* pointer = (char*)&history.entries[index];
    //     int size = string_length(pointer);
    //     memory_copy(pointer, buffer.data, size);
    //     buffer.data[size] = '\0';
    //     buffer.ptr = size;
    //     shell_write(pointer);
    // } else {
    //     memory_copy(buffer.hold, buffer.data, buffer.hold_size);
    //     buffer.data[buffer.hold_size] = '\0';
    //     buffer.ptr = buffer.hold_size;
    //     shell_write(buffer.data);
    // }
}

// Sends a character to be processed by the shell.
void shell_input(char c)
{
    // TODO: Handle scrolling.
    if (c == '\n') {
        cursor_pos.x = 0;
        cursor_pos.y++;
        run((const char*)buffer.data);
        // TODO: Handle history.
        clear_buffer();
        cursor_pos.x = string_length(INPUT_INDICATOR);
        syscall_write(0, cursor_pos.y, (char*)INPUT_INDICATOR);
    } else if (c == '\b') {
        if (buffer.ptr == 0) {
            return;
        }
        buffer.data[--buffer.ptr] = '\0';
        if (cursor_pos.x == 0) {
            cursor_pos.x = WIDTH - 1;
            cursor_pos.y -= 1;
        } else {
            cursor_pos.x--;
        }
        syscall_put(cursor_pos.x, cursor_pos.y, ' ');
    } else if (c == SCROLL_DOWN_KEY) {
        // TODO: Implement.
    } else if (c == SCROLL_UP_KEY) {
        // TODO: Implement.
    } else {
        if (buffer.ptr == INPUT_BUFFER_SIZE - 1) {
            return;
        }
        buffer.data[buffer.ptr] = c;
        buffer.data[++buffer.ptr] = '\0';
        syscall_put(cursor_pos.x, cursor_pos.y, c);
        cursor_pos.x++;
        if (cursor_pos.x == WIDTH) {
            cursor_pos.x = 0;
            cursor_pos.y++;
        }
    }

    // } else if (c == SHELL_SCROLL_DOWN_KEY) {
    //     if (history.scroll == 0 && !string_equal(buffer.data, buffer.hold)) {
    //         memory_copy(buffer.data, buffer.hold, buffer.ptr);
    //         buffer.hold_size = buffer.ptr;
    //         buffer.hold[buffer.ptr] = '\0';
    //     }

    //     if (history.scroll != history.count) {
    //         history.scroll++;
    //     }

    //     load_history_entry();

    // } else if (c == SHELL_SCROLL_UP_KEY) {
    //     if (history.scroll != 0) {
    //         history.scroll--;
    //     }
    //     load_history_entry();
}

// Prints a string to the shell.
Position shell_write(const char* string)
{
    assert(string, NULL_PARAMETER_ERROR(shell_write, string));
    int x = cursor_pos.x;
    int y = cursor_pos.y;
    int i = 0;
    while (string[i] != '\0') {
        if (string[i] == '\n') {
            x = 0;
            y++;
        } else {
            syscall_put(x, y, string[i]);
            x++;
        }
        i++;
    }
    Position end;
    end.x = x;
    end.y = y;
    return end;
}

void shell_output(const char* string)
{
    Position end = shell_write(string);
    cursor_pos.x = 0;
    cursor_pos.y = end.y + 1;
}