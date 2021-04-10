MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
PROJECT_DIR := $(dir $(MAKEFILE_PATH))
C_FILES := $(wildcard src/*.c)
C_FILENAMES := $(patsubst src/%.c,%,$(C_FILES))

define COMPILE_C

$(1).o: src/$(1).c
	mkdir -p build/
	i686-elf-gcc -I $(PROJECT_DIR)includes -c src/$(1).c -o build/$(1).o -ffreestanding -O2 -Wall -Wextra

endef

define ASSEMBLE_C

$(1).s: src/$(1).c
	mkdir -p build/
	i686-elf-gcc -I $(PROJECT_DIR)includes -c src/$(1).c -S -o build/$(1).s -ffreestanding -O2 -Wall -Wextra

endef

$(foreach file, $(C_FILENAMES), $(eval $(call COMPILE_C,$(file)))) 
$(foreach file, $(C_FILENAMES), $(eval $(call ASSEMBLE_C,$(file))))

assemble: $(foreach file, $(C_FILENAMES), $(file).s)
	$(info Assembly Generated!)

os: $(foreach file, $(C_FILENAMES), $(file).o) src/boot.asm
	mkdir -p bin/
	nasm -felf32 src/boot.asm -o build/boot.o
	i686-elf-gcc -T src/linker.ld -o bin/jasos.bin -ffreestanding -O2 -nostdlib build/boot.o $(foreach file, $(C_FILENAMES), build/$(file).o)

run: os
	qemu-system-i386 -kernel bin/jasos.bin -d int,cpu_reset -D out.txt
