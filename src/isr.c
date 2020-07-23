#include "isr.h"
#include "shell.h"

void isr_except0()
{
    shell_output_line("Divide zero exception!");
}

void isr_except1()
{
    shell_output_line("If you're here bad things have happened.");
}
