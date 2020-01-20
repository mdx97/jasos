#ifndef CLI_H
#define CLI_H

#include "terminal.h"

class CLI
{
    public:
        CLI(Terminal *term);
        void input(char c);
        void output(const char *string);

    private:
        Terminal *terminal;
        char buffer[256];
        int buffer_ptr;
        void ready_input();
        void clear_buffer();
};

#endif