# Just Another Silly Operating System

## Requirements
- GNU Make
- NASM

## How to Use
1. Build the bootloader.

    ```make bootloader```

2. Run the bootloader in your VM of choice. I use QEMU. (Don't run this on actual hardware you madman).

    ```qemu-system-i386 build/bootloader.img```