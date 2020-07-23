#ifndef ISR_H
#define ISR_H

#include "asm.h"

ASM_PROC(isr_except0_wrap);
ASM_PROC(isr_except1_wrap);
ASM_PROC(isr_except8_wrap);
ASM_PROC(isr_except13_wrap);
ASM_PROC(isr_except14_wrap);

void isr_except0();
void isr_except1();
void isr_except8();
void isr_except13();
void isr_except14();

#endif
