bits: src/bits.c
	i686-elf-gcc -c src/bits.c -o build/bits.o -ffreestanding -O2 -Wall -Wextra

boot: src/boot.asm
	nasm -felf32 src/boot.asm -o build/boot.o

gdt: src/gdt.c
	i686-elf-gcc -c src/gdt.c -o build/gdt.o -ffreestanding -O2 -Wall -Wextra

idt: src/idt.c
	i686-elf-gcc -c src/idt.c -o build/idt.o -ffreestanding -O2 -Wall -Wextra

kernel: src/kernel.c
	i686-elf-gcc -c src/kernel.c -o build/kernel.o -ffreestanding -O2 -Wall -Wextra

keyboard: src/keyboard.c
	i686-elf-gcc -c src/keyboard.c -o build/keyboard.o -ffreestanding -O2 -Wall -Wextra

shell: src/shell.c
	i686-elf-gcc -c src/shell.c -o build/shell.o -ffreestanding -O2 -Wall -Wextra

string: src/string.c
	i686-elf-gcc -c src/string.c -o build/string.o -ffreestanding -O2 -Wall -Wextra

utility: src/utility.c
	i686-elf-gcc -c src/utility.c -o build/utility.o -ffreestanding -O2 -Wall -Wextra

os: bits boot gdt idt kernel keyboard shell string utility
	i686-elf-gcc -T src/linker.ld -o bin/jasos.bin -ffreestanding -O2 -nostdlib build/bits.o build/boot.o build/gdt.o build/idt.o build/kernel.o build/keyboard.o build/shell.o build/string.o build/utility.o -lgcc

run: os
	qemu-system-i386 -kernel bin/jasos.bin -d int,cpu_reset -D out.txt