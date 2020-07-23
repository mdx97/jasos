#include "asm.h"
#include "bits.h"
#include "gdt.h"
#include "shell.h"
#include "utility.h"

#define GDT_SIZE 3

ASM_PROC(load_gdt);

GdtRegister GDTR;
GdtDescriptor gdt[GDT_SIZE];

/*
    Creates a GDT entry with the given arguments.

    Parameters:
    - base_address:             The 32-bit address of the segment this entry is describing.
    - limit:                    The maximum addressable unit for the segment.
    - access_byte:              The byte that contains various descriptor flags (see Intel documentation for more info).
    - flags:                    The 4-bit value that contains the granularity and word size bits.
*/
void create_gdt_entry(GdtDescriptor *descriptor, uint32_t base_address, uint32_t limit, uint8_t access_byte, uint8_t flags)
{
    descriptor->base1 = lsb(base_address, 16);
    descriptor->base2 = lsb(base_address >> 16, 8);
    descriptor->base3 = base_address >> 24;

    descriptor->limit = lsb(limit, 16);

    descriptor->access_byte = access_byte;
    descriptor->limit_and_flags = lsb(limit >> 16, 4) | flags << 4;
}

// Populates the GDTR with the size and address of the GDT.
void construct_gdtr(GdtDescriptor *table_pointer, GdtRegister *gdtr)
{
    gdtr->base_address = (uint32_t)table_pointer;
    gdtr->limit = GDT_SIZE * sizeof(GdtDescriptor) - 1;
}

// Initializes the GDT and loads it into memory.
void gdt_init()
{
    memory_fill(gdt, 0, sizeof(gdt));
    create_gdt_entry(&gdt[0], 0, 0x00000, 0x80, 0x4);
    create_gdt_entry(&gdt[1], 0, 0xFFFFF, 0x9A, 0xC);
    create_gdt_entry(&gdt[2], 0, 0xFFFFF, 0x93, 0xC);
    construct_gdtr(gdt, &GDTR);
    load_gdt();
}
