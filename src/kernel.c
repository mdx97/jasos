#include "asm.h"
#include "gdt.h"
#include "idt.h"
#include "kernel.h"
#include "keyboard.h"
#include "pic.h"
#include "shell.h"
#include "string.h"
#include "utility.h"

void shout()
{
    shell_writeline("ASM Shout!");
}

// Invokes a textual command against the kernel.
void system(const char *command)
{
    assert(command, NULL_PARAMETER_ERROR(system, command));

    if (string_equal(command, "clear")) {
        shell_clear();
    } else if (string_equal(command, "help")) {
        shell_writeline("The kernel supports the following built-in commands:\n- clear: Clears the screen\n- help: How did you get here?\n- system: Prints basic system information");
    } else if (string_equal(command, "system")) {
        shell_writeline("\n"
             " JJJJJJJJJ  AAAAAAAAA  SSSSSSSSS  OOOOOOOOO  SSSSSSSSS\n"
             " JJJJJJJJJ  AAAAAAAAA  SSSSSSSSS  OOOOOOOOO  SSSSSSSSS\n"
             "    JJJ     AAA   AAA  SSS        OOO   OOO  SSS      \n"
             "    JJJ     AAA   AAA  SSS        OOO   OOO  SSS      \n"
             "    JJJ     AAAAAAAAA  SSSSSSSSS  OOO   OOO  SSSSSSSSS\n"
             "    JJJ     AAA   AAA  SSSSSSSSS  OOO   OOO  SSSSSSSSS\n"
             "JJJ JJJ     AAA   AAA        SSS  OOO   OOO        SSS\n"
             "JJJ JJJ     AAA   AAA        SSS  OOO   OOO        SSS\n"
             "JJJJJJJ     AAA   AAA  SSSSSSSSS  OOOOOOOOO  SSSSSSSSS\n"
             "JJJJJJJ     AAA   AAA  SSSSSSSSS  OOOOOOOOO  SSSSSSSSS\n\n"
             "Kernel Version: 0.01\n"
        );
    } else if (!string_equal(command, "")) {
        shell_writeline("Command not found! Type help to view available commands.");
    }
}

// Crashes the system if the condition fails.
void assert(bool condition, const char *message)
{
    if (message == 0) {
        shell_clear();
        shell_writeline("Error in assert: parameter 'message' is a null pointer.");
        __asm__("hlt");
    }

    if (!condition) {
        shell_clear();
        shell_writeline(message);
        __asm__("hlt");
    }
}

// JASOS kernel entry point.
void kernel_main()
{
    shell_init();
    gdt_init();
    idt_init();
    pic_init();
    
    enable_hardware_interrupts();

    shell_write(SHELL_INPUT_INDICATOR);

    while (true);
}
