#include "keyboard.h"

char scancode_keymap[] = {
    '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 
    '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '\0',
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '\0', '\0', '\0', ' ' 
};

extern uint8_t read_ps2_status() __asm__("read_ps2_status");
extern uint8_t read_ps2_key() __asm__("read_ps2_key");

// Polls for a key until one is pressed, then returns the corresponding character value.
char read_key()
{
    while (true) {
        uint8_t kb_status = read_ps2_status();
        if ((kb_status & 0x1) == 1) {
            uint8_t scancode = read_ps2_key() - 1;
            return scancode_keymap[scancode];
        }
    }
}