#ifndef ASM_H
#define ASM_H

#define ASM_PROC(name) extern void name() __asm__(#name)
#define ASM_FNC(name, type) extern type name() __asm__(#name)

#endif
