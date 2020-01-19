#ifndef CLI_H
#define CLI_H

#include "terminal.h"

enum CLIMode { INPUT, OUTPUT };

class CLI
{
    public:
        CLI(Terminal *term);
        void input(char c);

    private:
        Terminal *terminal;
        CLIMode mode;
        char buffer[256];
        int buffer_ptr;
        void ready_input();
        void clear_buffer();
};

#endif