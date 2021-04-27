#include "string.h"
#include "syscalls.h"

#define HEIGHT 25
#define MAX_LINES 256
#define WIDTH 80

const char* PROMPT = "> ";

typedef struct position_t {
    int x;
    int y;
} Position;

Position cursor;
char output[MAX_LINES][WIDTH];
int offset = 0;

// Scrolls the contents of the shell down by one line.
void scroll()
{
    offset++;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            syscall_put(j, i, ' ');
        }
    }
    cursor.x = 0;
    cursor.y = 0;
    for (int i = 0; i < HEIGHT - 1; i++) {
        write(output[offset + i]);
        cursor.y++;
        cursor.x = 0;
    }
}

// Moves the cursor to the next line and respects screen bounds.
void nextline()
{
    cursor.x = 0;
    cursor.y++;
    if (cursor.y == HEIGHT) {
        scroll();
    }
}

// Writes a single character to the shell.
void puts(char c)
{
    syscall_put(cursor.x, cursor.y, c);
    output[cursor.y + offset][cursor.x] = c;
    cursor.x++;
    if (cursor.x == WIDTH) {
        nextline();
    }
}

// Writes a string to the shell.
void write(const char* string)
{
    char* ptr = (char*)string;
    while (*ptr != '\0') {
        puts(*ptr);
        ptr++;
    }
}

// Handles user input for the shell.
void shell_input(char c)
{
    if (c == '\n') {
        nextline();
        write(PROMPT);
    } else {
        puts(c);
    }
}

// Wrapper function for other parts of the kernel to send output to the shell.
// TODO: This should be removed once the program is moved into userspace.
void shell_output(const char* string)
{
    write(string);
}

// Initialization function that the kernel calls since this program is still a part of the kernel.
// TODO: Once this is moved into userspace, this should become a regular main function.
void shell_init()
{
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            output[i][j] = '\0';
        }
    }
    cursor.x = 0;
    cursor.y = 0;
}