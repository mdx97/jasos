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
#define HEIGHT 25
#define WIDTH 80

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

typedef struct t_position {
    int x;
    int y;
} Position;

InputBuffer buffer;
Position cursor_pos;
int offset;

Position shell_write(const char* string, Position position);

Position create_position(int x, int y)
{
    Position pos;
    pos.x = x;
    pos.y = y;
    return pos;
}

void hard_clear()
{
    // TODO: Need to wipe out ALL of lines, not just the ones currently being displayed.
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            syscall_put(i, j, ' ');
        }
    }
    cursor_pos.x = 0;
    cursor_pos.y = 0;
}

void soft_clear()
{
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            syscall_put(i, j, ' ');
        }
    }
}

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
        hard_clear();
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

void handle_scroll()
{
    if (cursor_pos.y >= HEIGHT) {
        syscall_scroll(cursor_pos.y - HEIGHT);
        cursor_pos.y = HEIGHT - 1;
    }
}

// Initializes the shell.
void shell_init()
{
    buffer.data[0] = '\0';
    buffer.hold[0] = '\0';
    buffer.ptr = 0;
    buffer.hold_size = 0;
    offset = 0;
    syscall_write(0, 0, (char*)INPUT_INDICATOR);
    cursor_pos.x = string_length(INPUT_INDICATOR);
    cursor_pos.y = 0;
}

// Sends a character to be processed by the shell.
void shell_input(char c)
{
    if (c == '\n') {
        cursor_pos.x = 0;
        cursor_pos.y++;
        run((const char*)buffer.data);
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
            handle_scroll();
        }
    }
}

// Prints a string to the shell.
Position shell_write(const char* string, Position position)
{
    assert(string, NULL_PARAMETER_ERROR(shell_write, string));
    int x = position.x;
    int y = position.y;
    int i = 0;
    while (string[i] != '\0') {
        if (string[i] == '\n') {
            x = 0;
            y++;
        } else {
            syscall_put(x, y, string[i]);
            x++;
            if (x == WIDTH) {
                x = 0;
                y++;
            }
        }
        handle_scroll();
        i++;
    }
    return create_position(x, y);
}

void shell_output(const char* string)
{
    Position end = shell_write(string, cursor_pos);
    cursor_pos.x = 0;
    cursor_pos.y = end.y + 1;
    handle_scroll();
}