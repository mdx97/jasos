#include "keyboard.h"
#include "asm.h"
#include "shell.h"
#include <stdint.h>

const int KEYBOARD_DATA_PORT = 0x60;

char scancode_keymap[] = {
    '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '\0',
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '\0', '\0', '\0', ' '};

void keydown()
{
    uint8_t scancode = in8(KEYBOARD_DATA_PORT) - 1;
    char c = scancode_keymap[scancode];
    if (c != '\0') {
        // TEMP: This should not directly invoke the shell once we have multitasking working.
        shell_input(c);
    }
}
