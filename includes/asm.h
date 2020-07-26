#ifndef ASM_H
#define ASM_H

#include <stdint.h>

#define ASM_PROC(name) extern void name() __asm__(#name)
#define ASM_FNC(name, type) extern type name() __asm__(#name)

uint8_t in8(uint16_t port);
void out8(uint16_t port, uint8_t data);
void enable_hardware_interrupts();
void remap_pic();

#endif
