#include "kernel.h"
#include "asm.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "pic.h"
#include "shell.h"
#include "string.h"
#include "syscalls.h"
#include "utility.h"
#include "vga.h"

// Crashes the system if the condition fails.
void assert(bool condition, const char* message)
{
    if (message == 0) {
        vga_init();
        vga_write(0, 0, "Error in assert: parameter 'message' is a null pointer.");
        __asm__("hlt");
    }
    if (!condition) {
        vga_init();
        vga_write(0, 0, (char*)message);
        __asm__("hlt");
    }
}

// JASOS kernel entry point.
void kernel_main()
{
    vga_init();
    gdt_init();
    idt_init();
    pic_init();
    enable_hardware_interrupts();
    shell_init();
    while (true)
        ;
}
