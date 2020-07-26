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

void isr_irq0();
void isr_irq1();
void isr_irq2();
void isr_irq3();
void isr_irq4();
void isr_irq5();
void isr_irq6();
void isr_irq7();
void isr_irq8();
void isr_irq9();
void isr_irq10();
void isr_irq11();
void isr_irq12();
void isr_irq13();
void isr_irq14();
void isr_irq15();

#endif
