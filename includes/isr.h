#ifndef ISR_H
#define ISR_H

#include "asm.h"

ASM_PROC(isr_except0_wrap);
ASM_PROC(isr_except1_wrap);

void isr_except0();
void isr_except1();

#endif
