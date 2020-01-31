#include "gdt.h"
#include "keyboard.h"
#include "shell.h"
#include "string.h"

extern void load_gdt() __asm__("load_gdt");

GdtRegister GDTR;
Shell *shell;

// Invokes a textual command against the kernel.
void system(const char *command)
{
    if (string_equal(command, "clear")) {
        shell->clear();
    } else if (string_equal(command, "help")) {
        shell->output("The kernel supports the following built-in commands:\n- clear: Clears the screen\n- help: How did you get here?\n- sys: Prints basic system information\n");
    } else if (string_equal(command, "sys")) {
        shell->output("\n"
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
        shell->output("Command not found! Type help to view available commands.\n");
    }
}

// JASOS kernel entry point.
extern "C" void kernel_main()
{
    CommandHistory cmd_history;
    Shell kshell{cmd_history, (volatile char*)0xB8000, 80, 25};
    shell = &kshell;
    shell->clear();

    Gdt gdt;
    gdt.add_entry(0, 0, 0, true, false, true, false, false, true);
    gdt.add_entry(0, 0xFFFFF, 0, false, true, true, true, false, true);
    gdt.add_entry(0, 0xFFFFF, 0, false, false, true, true, false, true);
    gdt.construct_gdtr(&GDTR);
    load_gdt();

    shell->ready_input();
    
    while (true) {
        char c = read_key();
        if (c != '\0')
            shell->input(c);
    } 
}
