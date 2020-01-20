#include "cli.h"
#include "kernel.h"

CLI::CLI(Terminal *term)
{
    terminal = term;
    buffer_ptr = 0;
    ready_input();
}

void CLI::input(char c)
{
    if (c == '\b') {
        if (buffer_ptr == 0) return;
        buffer[buffer_ptr--] = '\0';
        terminal->putchar('\b');
    } else if (c == '\n') {
        terminal->putchar('\n');
        system((const char *)buffer);
        clear_buffer();
        ready_input();
    } else {
        buffer[buffer_ptr] = c;
        buffer_ptr++;
        terminal->putchar(c);
    }
    
}

void CLI::output(const char *string)
{
    terminal->print(string);
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