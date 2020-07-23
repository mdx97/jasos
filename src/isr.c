#include "isr.h"
#include "shell.h"

void isr_except0()
{
    // TODO: We should try to recover from this by terminating the process if this exception is generated from userspace (aka not the kernel).
    shell_output_line("Divide by zero exception! Halting OS...");
    __asm__("hlt");
}

void isr_except1()
{
    shell_output_line("If you're here bad things have happened.");
}

void isr_except8()
{
    shell_output_line("Double Fault! Halting OS...");
    __asm__("hlt");
}

void isr_except13()
{
    // TODO: We should try to recover from this.
    shell_output_line("General Protection Fault! Halting OS...");
    __asm__("hlt");
}

void isr_except14()
{
    // TODO: We should try to recover from this.
    shell_output_line("Page Fault! Halting OS...");
    __asm__("hlt");
}