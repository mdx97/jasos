bootloader: src/bootloader.asm
	nasm -f elf32 src/bootloader.asm -o build/bootloader.o

god: src/god.c
	i686-elf-gcc -c src/god.c -o build/god.o -ffreestanding

os: bootloader god
	i686-elf-ld -Ttext=0x7c00 --oformat binary build/bootloader.o build/god.o -o bin/jasos.bin