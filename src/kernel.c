#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "shell.h"
#include "string.h"
#include "utility.h"

void shout()
{
    shell_output_line("ASM Shout!");
}

// Invokes a textual command against the kernel.
void system(const char *command)
{
    if (string_equal(command, "clear")) {
        shell_clear();
    } else if (string_equal(command, "help")) {
        shell_output_line("The kernel supports the following built-in commands:\n- clear: Clears the screen\n- help: How did you get here?\n- sys: Prints basic system information");
    } else if (string_equal(command, "sys")) {
        shell_output_line("\n"
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
        shell_output_line("Command not found! Type help to view available commands.");
    }
}

// JASOS kernel entry point.
void kernel_main()
{
    shell_init();
    gdt_init();
    idt_init();

    // uint8_t a = 10;
    // uint8_t b = 0;
    // uint8_t d = a / b;
    // char string[9];
    // uint_to_string((uint32_t)d, (char *)&string);
    
    // shell_output_line((const char *)string);

    shell_ready_input();

    while (true) {
        char c = read_key();
        if (c != '\0')
            shell_input(c);
    } 
}
