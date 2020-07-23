MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
PROJECT_DIR := $(dir $(MAKEFILE_PATH))
C_FILES := $(patsubst src/%.c,%,$(wildcard src/*.c))

define COMPILE_C

$(1).o: src/$(1).c
	i686-elf-gcc -I $(PROJECT_DIR)includes -c src/$(1).c -o build/$(1).o -ffreestanding -O2 -Wall -Wextra

endef

$(foreach file, $(C_FILES), $(eval $(call COMPILE_C,$(file))))

boot: src/boot.asm
	nasm -felf32 src/boot.asm -o build/boot.o

os: $(foreach file, $(C_FILES), $(file).o) boot
	i686-elf-gcc -T src/linker.ld -o bin/jasos.bin -ffreestanding -O2 -nostdlib build/boot.o $(foreach file, $(C_FILES), build/$(file).o) -lgcc

run: os
	qemu-system-i386 -kernel bin/jasos.bin -d int,cpu_reset -D out.txt