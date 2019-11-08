#include "gdt.h"
#include "terminal.h"

void sermon_on_the_mount()
{
    terminal_print("\tHello\n\tWorld!");
}

// JASOS kernel entry point.
void let_there_be_light()
{
    terminal_init();
    gdt_init();
    sermon_on_the_mount();
}
