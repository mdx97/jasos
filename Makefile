bootloader: src/bootloader.asm
	nasm -f bin -o build/bootloader.img src/bootloader.asm
