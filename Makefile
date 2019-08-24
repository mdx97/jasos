bootloader: src/bootloader.asm
	nasm -f elf32 src/bootloader.asm -o build/bootloader.o

god: bootloader
	i686-elf-gcc -c src/god.c -o build/god.o -ffreestanding -nostdlib -lgcc

os: bootloader god
	i686-elf-ld -Ttext=0x7c00 --oformat binary build/bootloader.o -o bin/jasos.bin