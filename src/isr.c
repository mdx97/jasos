#include "isr.h"
#include "keyboard.h"
#include "shell.h"

void exception_halt(const char *description)
{
    shell_write(description);
    shell_writeline("! Halting OS...");
    __asm__("hlt");
}

// Exception routines.

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

// IRQ routines.

void isr_irq0()
{

}

void isr_irq1()
{
    keydown();
}

void isr_irq2()
{
    
}

void isr_irq3()
{

}

void isr_irq4()
{
    
}

void isr_irq5()
{
    
}

void isr_irq6()
{

}

void isr_irq7()
{
    
}

void isr_irq8()
{

}

void isr_irq9()
{
    
}

void isr_irq10()
{
    
}

void isr_irq11()
{

}

void isr_irq12()
{
    
}

void isr_irq13()
{
    
}

void isr_irq14()
{
    
}

void isr_irq15()
{
    
}
