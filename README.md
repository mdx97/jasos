# Just Another Silly Operating System

## Requirements
- GNU Make
- NASM

## How to Use
1. Build the OS.

    ```make os```

2. Run Jasos in your VM of choice. I use QEMU. (Don't run this on actual hardware you madman).

    ```qemu-system-i386 -drive file=bin/jasos.bin,index=0,media=disk,format=raw```