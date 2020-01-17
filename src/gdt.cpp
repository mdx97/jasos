#include "gdt.h"
#include "bits.h"

Gdt::Gdt()
{
    next_descriptor = 0;
}

/*
    Properly adds a GDT Entry with the given arguments.

    Parameters:
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
int Gdt::add_entry(
    uint32_t base_address, uint32_t limit, uint8_t ring,
    bool is_system_segment, bool is_executable, bool grows_up, bool read_write,
    bool uses_page_granularity, bool is_32_bit
){
    if (ring != 0 && ring != 3) return 1;
    if (limit > 0xFFFFF) return 2;

    GdtDescriptor *descriptor = &descriptors[next_descriptor];
    descriptor->base1 = lsb(base_address, 16);
    descriptor->base2 = lsb(base_address >> 16, 8);
    descriptor->base3 = base_address >> 24;

    descriptor->limit = lsb(limit, 16);
    descriptor->limit_and_flags = (limit >> 28) + (is_32_bit << 6) + (uses_page_granularity << 7);
    
    descriptor->access_byte = (read_write << 1) + (!grows_up << 2) + (is_executable << 3) + (!is_system_segment << 4) + (lsb(ring, 2) << 5) + (1 << 7);
    return 0;
}

void Gdt::construct_gdtr(GdtRegister *gdtr)
{
    gdtr->base_address = (uint32_t)&descriptors;
    gdtr->limit = 3 * sizeof(GdtDescriptor);
}
