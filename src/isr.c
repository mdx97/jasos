#include "isr.h"
#include "shell.h"

void exception_halt(const char *description)
{
    shell_output(description);
    shell_output_line("! Halting OS...");
    __asm__("hlt");
}

// TODO: (Almost) all of these exception handlers contain placeholder code and are irrelevant until the kernel 
// is more mature. Every one of these exception handlers should be revisted eventually.

void isr_except0()
{
    exception_halt("Divide By Zero Fault");
}

void isr_except1()
{
    exception_halt("Debug Exception (not supported)");
}

void isr_except2()
{
    exception_halt("NMI (not supported)");
}

void isr_except3()
{
    exception_halt("Breakpoint Exception (not supported)");
}

void isr_except4()
{
    exception_halt("Overflow Exception (not supported)");
}

void isr_except5()
{
    exception_halt("BOUND Range Exceeded Fault");
}

void isr_except6()
{
    exception_halt("Invalid Opcode Fault");
}

void isr_except7()
{
    exception_halt("Device Not Available Fault");
}

void isr_except8()
{
    exception_halt("Double Fault");
}

void isr_except9()
{
    exception_halt("Coprocessor Segment Overrun");
}

void isr_except10()
{
    exception_halt("Invalid TSS Fault");
}

void isr_except11()
{
    exception_halt("Segment Not Present Fault");
}

void isr_except12()
{
    exception_halt("Stack Fault");
}

void isr_except13()
{
    exception_halt("General Protection Fault");
}

void isr_except14()
{
    exception_halt("Page Fault");
}

void isr_except16()
{
    exception_halt("x87 FPU Floating Point Error Fault");
}

void isr_except17()
{
    exception_halt("Alignment Check Fault");
}

void isr_except18()
{
    exception_halt("Machine Check Exception");
}

void isr_except19()
{
    exception_halt("SIMD Floating-Point Fault");
}
