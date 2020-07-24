#ifndef ISR_H
#define ISR_H

#include "asm.h"

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

void isr_except0();
void isr_except1();
void isr_except2();
void isr_except3();
void isr_except4();
void isr_except5();
void isr_except6();
void isr_except7();
void isr_except8();
void isr_except9();
void isr_except10();
void isr_except11();
void isr_except12();
void isr_except13();
void isr_except14();
void isr_except16();
void isr_except17();
void isr_except18();
void isr_except19();

#endif
