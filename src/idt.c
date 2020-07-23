#include <stdbool.h>
#include "bits.h"
#include "idt.h"
#include "shell.h"
#include "utility.h"

#define IDT_SIZE 256

const int GATE_TYPE_TASK_32 = 0x5;
const int GATE_TYPE_INTERRUPT_16 = 0x6;
const int GATE_TYPE_INTERRUPT_32 = 0xE;
const int GATE_TYPE_TRAP_16 = 0x7;
const int GATE_TYPE_TRAP_32 = 0xF;

extern void load_idt() __asm__("load_idt");

IdtRegister IDTR;
IdtDescriptor idt[IDT_SIZE];

/*
    Creates an IDT entry with the given arguments.

    Parameters:
    - offset:               The address of the ISR.
    - selector:             Segment selector for the ISR's CS.
    - gate_type:            The IDT gate type.
    - dpl:                  Descriptor privilege level. 2-bit value.
*/
void create_idt_entry(IdtDescriptor *descriptor, uint32_t offset, uint16_t selector, uint8_t gate_type, uint8_t dpl)
{
    descriptor->offset_low = lsb(offset, 16);
    descriptor->offset_high = offset >> 16;
    descriptor->selector = selector;
    descriptor->unused = 0;
    
    descriptor->data = gate_type;
    descriptor->data |= (gate_type == GATE_TYPE_TASK_32) << 4;
    descriptor->data |= dpl << 5;
    descriptor->data |= 1 << 7;
}

void temp()
{
    shell_output_line("Hello world!");
}

// Populates the IDTR with the size and address of the IDT.
void construct_idtr(IdtDescriptor *table_pointer, IdtRegister *idtr)
{
    idtr->base_address = (uint32_t)table_pointer;
    idtr->limit = IDT_SIZE * sizeof(IdtDescriptor);
}

// Initializes the IDT and loads it into memory.
void idt_init()
{
    create_idt_entry(&idt[0], (uint32_t)&temp, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[1], (uint32_t)&temp, 0x8, GATE_TYPE_TRAP_32, 0);
    construct_idtr(idt, &IDTR);
    load_idt();
}
