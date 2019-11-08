bits: src/bits.c
	i686-elf-gcc -c src/bits.c -o build/bits.o -ffreestanding

boot: src/boot.asm
	nasm -felf32 src/boot.asm -o build/boot.o

gdt: src/gdt.c
	i686-elf-gcc -c src/gdt.c -o build/gdt.o -ffreestanding

god: src/god.c
	i686-elf-gcc -c src/god.c -o build/god.o -ffreestanding

terminal: src/terminal.c
	i686-elf-gcc -c src/terminal.c -o build/terminal.o -ffreestanding

os: bits boot gdt god terminal
	i686-elf-gcc -T src/linker.ld -o bin/jasos.bin -ffreestanding -O2 -nostdlib build/bits.o build/boot.o build/gdt.o build/god.o build/terminal.o -lgcc

run: os
	qemu-system-i386 -kernel bin/jasos.bin