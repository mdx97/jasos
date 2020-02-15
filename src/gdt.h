#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include <stdbool.h>

typedef struct __attribute__((__packed__)) t_gdt_register {
    uint16_t limit;
    uint32_t base_address;
} GdtRegister;

typedef struct __attribute__((__packed__)) t_gdt_descriptor {
    uint16_t limit;
    uint16_t base1;
    uint8_t base2;
    uint8_t access_byte;
    uint8_t limit_and_flags;
    uint8_t base3;
} GdtDescriptor;

void gdt_init();

#endif
