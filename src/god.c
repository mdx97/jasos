#include "terminal.h"

void sermon_on_the_mount()
{
    terminal_putc('H');
    terminal_putc('e');
    terminal_putc('l');
    terminal_putc('l');
    terminal_putc('o');
    terminal_putc(' ');
    terminal_putc('W');
    terminal_putc('o');
    terminal_putc('r');
    terminal_putc('l');
    terminal_putc('d');
    terminal_putc('!');
}

// JASOS kernel entry point.
void let_there_be_light()
{
    terminal_init();
    sermon_on_the_mount();
}
