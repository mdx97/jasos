#include "cli.h"
#include "gdt.h"
#include "keyboard.h"
#include "port.h"
#include "terminal.h"

extern void load_gdt() __asm__("load_gdt");

GdtRegister GDTR;

// JASOS kernel entry point.
extern "C" void kernel_main()
{
    Terminal terminal{(volatile char*)0xB8000, 80, 25};
    terminal.clear();

    Gdt gdt;
    gdt.add_entry(0, 0, 0, true, false, true, false, false, true);
    gdt.add_entry(0, 0xFFFFF, 0, false, true, true, true, false, true);
    gdt.add_entry(0, 0xFFFFF, 0, false, false, true, true, false, true);
    gdt.construct_gdtr(&GDTR);
    load_gdt();

    CLI cli{&terminal};

    while (true) {
        char c = read_key();
        if (c != '\0')
            cli.input(c);
    } 

    //for (;;);
}
