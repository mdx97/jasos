#ifndef GDT_H
#define GDT_H

#include <stdint.h>

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

class Gdt
{
    public:
        Gdt();
        void construct_gdtr(GdtRegister *gdtr);
        int add_entry(uint32_t base_address, uint32_t limit, uint8_t ring,
            bool is_system_segment, bool is_executable, bool grows_up, bool read_write,
            bool uses_page_granularity, bool is_32_bit);
        GdtDescriptor descriptors[3];
    
    private:
        int next_descriptor;
};

#endif
