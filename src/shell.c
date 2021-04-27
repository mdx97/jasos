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

void nextline()
{
    cursor.x = 0;
    cursor.y++;
    if (cursor.y == HEIGHT) {
        scroll();
    }
}

void puts(char c)
{
    syscall_put(cursor.x, cursor.y, c);
    output[cursor.y + offset][cursor.x] = c;
    cursor.x++;
    if (cursor.x == WIDTH) {
        nextline();
    }
}

void write(const char* string)
{
    char* ptr = (char*)string;
    while (*ptr != '\0') {
        puts(*ptr);
        ptr++;
    }
}

void prompt()
{
    write(PROMPT);
}

void shell_init()
{
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            output[i][j] = '\0';
        }
    }
    cursor.x = 0;
    cursor.y = 0;
    prompt();
}

void shell_input(char c)
{
    if (c == '\n') {
        nextline();
        prompt();
    } else {
        puts(c);
    }
}

void shell_output(const char* string)
{
    write(string);
}