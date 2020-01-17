bits: src/bits.cpp
	i686-elf-g++ -c src/bits.cpp -o build/bits.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

boot: src/boot.asm
	nasm -felf32 src/boot.asm -o build/boot.o

gdt: src/gdt.cpp
	i686-elf-g++ -c src/gdt.cpp -o build/gdt.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

kernel: src/kernel.cpp
	i686-elf-g++ -c src/kernel.cpp -o build/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

terminal: src/terminal.cpp
	i686-elf-g++ -c src/terminal.cpp -o build/terminal.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

os: bits boot gdt kernel terminal
	i686-elf-gcc -T src/linker.ld -o bin/jasos.bin -ffreestanding -O2 -nostdlib build/bits.o build/boot.o build/gdt.o build/kernel.o build/terminal.o -lgcc

run: os
	qemu-system-i386 -kernel bin/jasos.bin