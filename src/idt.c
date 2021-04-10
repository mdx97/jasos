#include "idt.h"
#include "asm.h"
#include "bits.h"
#include "shell.h"
#include "utility.h"
#include <stdbool.h>
#include <stdint.h>

#define IDT_SIZE 256

const int GATE_TYPE_TASK_32 = 0x5;
const int GATE_TYPE_INTERRUPT_16 = 0x6;
const int GATE_TYPE_INTERRUPT_32 = 0xE;
const int GATE_TYPE_TRAP_16 = 0x7;
const int GATE_TYPE_TRAP_32 = 0xF;

ASM_PROC(isr_except0_wrap);
ASM_PROC(isr_except1_wrap);
ASM_PROC(isr_except2_wrap);
ASM_PROC(isr_except3_wrap);
ASM_PROC(isr_except4_wrap);
ASM_PROC(isr_except5_wrap);
ASM_PROC(isr_except6_wrap);
ASM_PROC(isr_except7_wrap);
ASM_PROC(isr_except8_wrap);
ASM_PROC(isr_except9_wrap);
ASM_PROC(isr_except10_wrap);
ASM_PROC(isr_except11_wrap);
ASM_PROC(isr_except12_wrap);
ASM_PROC(isr_except13_wrap);
ASM_PROC(isr_except14_wrap);
ASM_PROC(isr_except16_wrap);
ASM_PROC(isr_except17_wrap);
ASM_PROC(isr_except18_wrap);
ASM_PROC(isr_except19_wrap);

ASM_PROC(isr_irq0_wrap);
ASM_PROC(isr_irq1_wrap);
ASM_PROC(isr_irq2_wrap);
ASM_PROC(isr_irq3_wrap);
ASM_PROC(isr_irq4_wrap);
ASM_PROC(isr_irq5_wrap);
ASM_PROC(isr_irq6_wrap);
ASM_PROC(isr_irq7_wrap);
ASM_PROC(isr_irq8_wrap);
ASM_PROC(isr_irq9_wrap);
ASM_PROC(isr_irq10_wrap);
ASM_PROC(isr_irq11_wrap);
ASM_PROC(isr_irq12_wrap);
ASM_PROC(isr_irq13_wrap);
ASM_PROC(isr_irq14_wrap);
ASM_PROC(isr_irq15_wrap);

ASM_PROC(load_idt);

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
void create_idt_entry(IdtDescriptor* descriptor, uint32_t offset, uint16_t selector, uint8_t gate_type, uint8_t dpl)
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

// Initializes the IDT and loads it into memory.
void idt_init()
{
    memory_fill(idt, 0, sizeof(idt));

    create_idt_entry(&idt[0], (uint32_t)isr_except0_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[1], (uint32_t)isr_except1_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[2], (uint32_t)isr_except2_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[3], (uint32_t)isr_except3_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[4], (uint32_t)isr_except4_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[5], (uint32_t)isr_except5_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[6], (uint32_t)isr_except6_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[7], (uint32_t)isr_except7_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[8], (uint32_t)isr_except8_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[9], (uint32_t)isr_except9_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[10], (uint32_t)isr_except10_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[11], (uint32_t)isr_except11_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[12], (uint32_t)isr_except12_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[13], (uint32_t)isr_except13_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[14], (uint32_t)isr_except14_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[16], (uint32_t)isr_except16_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[17], (uint32_t)isr_except17_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[18], (uint32_t)isr_except18_wrap, 0x8, GATE_TYPE_TRAP_32, 0);
    create_idt_entry(&idt[19], (uint32_t)isr_except19_wrap, 0x8, GATE_TYPE_TRAP_32, 0);

    create_idt_entry(&idt[32], (uint32_t)isr_irq0_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[33], (uint32_t)isr_irq1_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[34], (uint32_t)isr_irq2_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[35], (uint32_t)isr_irq3_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[36], (uint32_t)isr_irq4_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[37], (uint32_t)isr_irq5_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[38], (uint32_t)isr_irq6_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[39], (uint32_t)isr_irq7_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[40], (uint32_t)isr_irq8_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[41], (uint32_t)isr_irq9_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[42], (uint32_t)isr_irq10_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[43], (uint32_t)isr_irq11_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[44], (uint32_t)isr_irq12_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[45], (uint32_t)isr_irq13_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[46], (uint32_t)isr_irq14_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);
    create_idt_entry(&idt[47], (uint32_t)isr_irq15_wrap, 0x8, GATE_TYPE_INTERRUPT_32, 0);

    IDTR.base_address = (uint32_t)idt;
    IDTR.limit = IDT_SIZE * sizeof(IdtDescriptor);

    load_idt();
}
