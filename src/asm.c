#include "asm.h"

uint8_t in8(uint16_t port)
{
    uint8_t result;
    __asm__ volatile ("in %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void out8(uint16_t port, uint8_t data)
{
    __asm__ volatile ("out %0, %1" :: "a"(data), "Nd"(port));
}

void enable_hardware_interrupts()
{
    __asm__ ("sti");
}
