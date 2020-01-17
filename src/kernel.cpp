//#include "gdt.h"
#include "terminal.h"

// JASOS kernel entry point.
extern "C" void kernel_main()
{
    Terminal terminal{(volatile char*)0xB8000, 80, 25};
    terminal.clear();
    //gdt_init();
    terminal.print("Hello\nWorld!");
}