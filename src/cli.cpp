#include "cli.h"

CLI::CLI(Terminal *term)
{
    terminal = term;
    mode = INPUT;
    buffer_ptr = 0;
    ready_input();
}

void CLI::input(char c)
{
    if (mode != INPUT) return;
    if (c == '\b') {
        if (buffer_ptr == 0) return;
        buffer[buffer_ptr--] = '\0';
        terminal->putchar('\b');
    } else if (c == '\n') {
        // TODO: try to execute text in buffer.
        clear_buffer();
        terminal->putchar('\n');
        ready_input();
    } else {
        buffer[buffer_ptr] = c;
        buffer_ptr++;
        terminal->putchar(c);
    }
    
}

void CLI::ready_input()
{
    terminal->print(">>> ");
}

void CLI::clear_buffer()
{
    buffer_ptr = 0;
    for (int i = 0; i < 256; i++)
        buffer[i] = '\0';
}