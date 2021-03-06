; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot
 
; Declare a multiboot header that marks the program as a kernel. These are magic
; values that are documented in the multiboot standard. The bootloader will
; search for this signature in the first 8 KiB of the kernel file, aligned at a
; 32-bit boundary. The signature is in its own section so the header can be
; forced to be within the first 8 KiB of the kernel file.
section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
 
; The multiboot standard does not define the value of the stack pointer register
; (esp) and it is up to the kernel to provide a stack. This allocates room for a
; small stack by creating a symbol at the bottom of it, then allocating 16384
; bytes for it, and finally creating a symbol at the top. The stack grows
; downwards on x86. The stack is in its own section so it can be marked nobits,
; which means the kernel file is smaller because it does not contain an
; uninitialized stack. The stack on x86 must be 16-byte aligned according to the
; System V ABI standard and de-facto extensions. The compiler will assume the
; stack is properly aligned and failure to align the stack will result in
; undefined behavior.
section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:
 
; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded. It
; doesn't make sense to return from this function as the bootloader is gone.
; Declare _start as a function symbol with the given symbol size.
section .text
global _start:function (_start.end - _start)
_start:
	; The bootloader has loaded us into 32-bit protected mode on a x86
	; machine. Interrupts are disabled. Paging is disabled. The processor
	; state is as defined in the multiboot standard. The kernel has full
	; control of the CPU. The kernel can only make use of hardware features
	; and any code it provides as part of itself. There's no printf
	; function, unless the kernel provides its own <stdio.h> header and a
	; printf implementation. There are no security restrictions, no
	; safeguards, no debugging mechanisms, only what the kernel provides
	; itself. It has absolute and complete power over the
	; machine.
 
	; To set up a stack, we set the esp register to point to the top of our
	; stack (as it grows downwards on x86 systems). This is necessarily done
	; in assembly as languages such as C cannot function without a stack.
	mov esp, stack_top
 
	; This is a good place to initialize crucial processor state before the
	; high-level kernel is entered. It's best to minimize the early
	; environment where crucial features are offline. Note that the
	; processor is not fully initialized yet: Features such as floating
	; point instructions and instruction set extensions are not initialized
	; yet. The GDT should be loaded here. Paging should be enabled here.
	; C++ features such as global constructors and exceptions will require
	; runtime support to work as well.
 
.disable_cursor:
	mov dx, 0x3D4
	mov al, 0xA
	out dx, al
 
	inc dx
	mov al, 0x20
	out dx, al

	; Enter the high-level kernel. The ABI requires the stack is 16-byte
	; aligned at the time of the call instruction (which afterwards pushes
	; the return pointer of size 4 bytes). The stack was originally 16-byte
	; aligned above and we've since pushed a multiple of 16 bytes to the
	; stack since (pushed 0 bytes so far) and the alignment is thus
	; preserved and the call is well defined.
        ; note, that if you are building on Windows, C functions may have "_" prefix in assembly: _kernel_main
	extern kernel_main
	call kernel_main

.end:

global load_gdt:function
load_gdt:
	extern GDTR
	lgdt [GDTR]

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	jmp 0x08:.end

.end:
	ret

global load_idt:function
load_idt:
	extern IDTR
	lidt [IDTR]
	ret

; TODO: Should refactor these so that we remove duplicated code.

extern isr_except0
extern isr_except1
extern isr_except2
extern isr_except3
extern isr_except4
extern isr_except5
extern isr_except6
extern isr_except7
extern isr_except8
extern isr_except9
extern isr_except10
extern isr_except11
extern isr_except12
extern isr_except13
extern isr_except14
extern isr_except16
extern isr_except17
extern isr_except18
extern isr_except19

global isr_except0_wrap:function
isr_except0_wrap:
	call isr_except0

global isr_except1_wrap:function
isr_except1_wrap:
	call isr_except1

global isr_except2_wrap:function
isr_except2_wrap:
	;pushad
	;cld
	call isr_except2
	;popad
	;iret

global isr_except3_wrap:function
isr_except3_wrap:
	call isr_except3
	
global isr_except4_wrap:function
isr_except4_wrap:
	call isr_except4

global isr_except5_wrap:function
isr_except5_wrap:
	call isr_except5

global isr_except6_wrap:function
isr_except6_wrap:
	call isr_except6

global isr_except7_wrap:function
isr_except7_wrap:
	call isr_except7
	
global isr_except8_wrap:function
isr_except8_wrap:
	call isr_except8

global isr_except9_wrap:function
isr_except9_wrap:
	call isr_except9

global isr_except10_wrap:function
isr_except10_wrap:
	call isr_except10

global isr_except11_wrap:function
isr_except11_wrap:
	call isr_except11

global isr_except12_wrap:function
isr_except12_wrap:
	call isr_except13

global isr_except13_wrap:function
isr_except13_wrap:
	call isr_except13

global isr_except14_wrap:function
isr_except14_wrap:
	call isr_except14

global isr_except16_wrap:function
isr_except16_wrap:
	call isr_except16

global isr_except17_wrap:function
isr_except17_wrap:
	call isr_except17

global isr_except18_wrap:function
isr_except18_wrap:
	call isr_except18

global isr_except19_wrap:function
isr_except19_wrap:
	call isr_except19

extern isr_irq0
extern isr_irq1
extern isr_irq2
extern isr_irq3
extern isr_irq4
extern isr_irq5
extern isr_irq6
extern isr_irq7
extern isr_irq8
extern isr_irq9
extern isr_irq10
extern isr_irq11
extern isr_irq12
extern isr_irq13
extern isr_irq14
extern isr_irq15

eoi:
	mov al, 0x20
	out 0x20, al
	ret

global isr_irq0_wrap:function
isr_irq0_wrap:
	pushad
	;cld
	call isr_irq0
	popad
	call eoi
	iret

global isr_irq1_wrap:function
isr_irq1_wrap:
	pushad
	;cld
	call isr_irq1
	popad
	call eoi
	iret

global isr_irq2_wrap:function
isr_irq2_wrap:
	pushad
	;cld
	call isr_irq2
	popad
	call eoi
	iret

global isr_irq3_wrap:function
isr_irq3_wrap:
	pushad
	;cld
	call isr_irq3
	popad
	call eoi
	iret

global isr_irq4_wrap:function
isr_irq4_wrap:
	pushad
	;cld
	call isr_irq4
	popad
	call eoi
	iret

global isr_irq5_wrap:function
isr_irq5_wrap:
	pushad
	;cld
	call isr_irq5
	popad
	call eoi
	iret

global isr_irq6_wrap:function
isr_irq6_wrap:
	pushad
	;cld
	call isr_irq6
	popad
	call eoi
	iret

global isr_irq7_wrap:function
isr_irq7_wrap:
	pushad
	;cld
	call isr_irq7
	popad
	call eoi
	iret

global isr_irq8_wrap:function
isr_irq8_wrap:
	pushad
	;cld
	call isr_irq8
	popad
	call eoi
	iret

global isr_irq9_wrap:function
isr_irq9_wrap:
	pushad
	;cld
	call isr_irq9
	popad
	call eoi
	iret

global isr_irq10_wrap:function
isr_irq10_wrap:
	pushad
	;cld
	call isr_irq10
	popad
	call eoi
	iret

global isr_irq11_wrap:function
isr_irq11_wrap:
	pushad
	;cld
	call isr_irq11
	popad
	call eoi
	iret

global isr_irq12_wrap:function
isr_irq12_wrap:
	pushad
	;cld
	call isr_irq12
	popad
	call eoi
	iret

global isr_irq13_wrap:function
isr_irq13_wrap:
	pushad
	;cld
	call isr_irq13
	popad
	call eoi
	iret

global isr_irq14_wrap:function
isr_irq14_wrap:
	pushad
	;cld
	call isr_irq14
	popad
	call eoi
	iret

global isr_irq15_wrap:function
isr_irq15_wrap:
	pushad
	;cld
	call isr_irq15
	popad
	call eoi
	iret
