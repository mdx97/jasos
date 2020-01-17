#include <stdint.h>
#include <stdbool.h>
#include "bits.h"

#define GDT_SIZE 3

typedef struct __attribute__((__packed__)) t_gdt_descriptor {
    uint16_t limit;
    uint32_t base_address;
} GdtDescriptor;

typedef struct __attribute__((__packed__)) t_gdt_entry {
    uint16_t limit;
    uint16_t base1;
    uint8_t base2;
    uint8_t access_byte;
    uint8_t limit_and_flags;
    uint8_t base3;
} GdtEntry;

GdtEntry gdt[GDT_SIZE];
GdtDescriptor gdt_descriptor;

/*
    Properly sets up a GDT Entry with the given arguments.

    Parameters:
    - gdt_entry*:               The struct that the function will operate on.
    - base_address:             The 32 bit address of the segment this entry is describing.
    - limit:                    The maximum addressable unit for the segment.
    - ring:                     The ring level.
                                Can be 0 (for kernel layer) or 3 (for application layer).
    - is_system_segment:        Whether the segment is a system segment (as opposed to a code or data segment).
    - is_executable:            Whether the segment is executable.
    - grows_up:                 Segment grows upwards if true, downwards if false.
    - read_write:               Whether the segment is readable or writable for code or data selectors, respectively.
    - uses_page_granularity:    Segment uses page granularity if true, byte granularity if false.
    - is_32_bit:                Segment is 32 bit if true, 16 bit if false.

    Returns:
    - 0: Successful.
    - 1: Invalid ring size.
    - 2: Limit larger than 20 bits.
*/
int create_gdt_entry(
    GdtEntry *gdt_entry, uint32_t base_address, uint32_t limit, uint8_t ring,
    bool is_system_segment, bool is_executable, bool grows_up, bool read_write,
    bool uses_page_granularity, bool is_32_bit
){
    if (ring != 0 && ring != 3) return 1;
    if (limit > 0xFFFFF) return 2;

    gdt_entry->base1 = lsb(base_address, 16);
    gdt_entry->base2 = lsb(base_address >> 16, 8);
    gdt_entry->base3 = base_address >> 24;

    gdt_entry->limit = lsb(limit, 16);
    gdt_entry->limit_and_flags = (limit >> 28) + (is_32_bit << 6) + (uses_page_granularity << 7);
    
    gdt_entry->access_byte = (read_write << 1) + (!grows_up << 2) + (is_executable << 3) + (!is_system_segment << 4) + (lsb(ring, 2) << 5) + (1 << 7);
    return 0;
}

void gdt_init()
{
    GdtEntry empty, cs, ds;
    create_gdt_entry(&empty, 0, 0, 0, true, false, true, false, false, true);
    create_gdt_entry(&cs, 0, 0xFFFFF, 0, false, true, true, true, false, true);
    create_gdt_entry(&ds, 0, 0xFFFFF, 0, false, false, true, true, false, true);

    gdt[0] = empty;
    gdt[1] = cs;
    gdt[2] = ds;

    gdt_descriptor.base_address = (uint32_t)&gdt;
    gdt_descriptor.limit = GDT_SIZE * sizeof(GdtEntry);
}