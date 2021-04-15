#include "pic.h"
#include "asm.h"

const uint8_t PIC_MASTER_COMMAND_PORT = 0x20;
const uint8_t PIC_MASTER_DATA_PORT = 0x21;
const uint8_t PIC_SLAVE_COMMAND_PORT = 0xA0;
const uint8_t PIC_SLAVE_DATA_PORT = 0xA1;

void pic_init()
{
    // Begin initialization sequence.
    out8(PIC_MASTER_COMMAND_PORT, 0x11);
    out8(PIC_SLAVE_COMMAND_PORT, 0x11);
    // Set the interrupt vectors for our IRQs to something that doesn't collide with Intel reserved interrupts.
    out8(PIC_MASTER_DATA_PORT, 0x20);
    out8(PIC_SLAVE_DATA_PORT, 0x28);
    // Master/slave configuration.
    out8(PIC_MASTER_DATA_PORT, 0x04);
    out8(PIC_SLAVE_DATA_PORT, 0x02);
    // Environment configuration (set 8086 mode).
    out8(PIC_MASTER_DATA_PORT, 0x01);
    out8(PIC_SLAVE_DATA_PORT, 0x01);
    // Enable all IRQ lines.
    out8(PIC_MASTER_DATA_PORT, 0x0);
    out8(PIC_SLAVE_DATA_PORT, 0x0);
}
