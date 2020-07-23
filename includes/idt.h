#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct __attribute__((__packed__)) t_idt_register {
    uint16_t limit;
    uint32_t base_address;
} IdtRegister;

typedef struct __attribute__((__packed__)) t_idt_descriptor {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t unused;
    uint8_t data;
    uint16_t offset_high;
} IdtDescriptor;

void idt_init();

#endif
