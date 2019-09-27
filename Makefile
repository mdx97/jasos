boot: src/boot.asm
	nasm -felf32 src/boot.asm -o build/boot.o

god: src/god.c
	i686-elf-gcc -c src/god.c -o build/god.o -ffreestanding

os: boot god
	i686-elf-gcc -T src/linker.ld -o bin/jasos.bin -ffreestanding -O2 -nostdlib build/boot.o build/god.o -lgcc