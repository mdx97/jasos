# Just Another Silly Operating System

## Requirements
- GNU Make
- NASM
- Cross compiler toolchain (see [OSDev Wiki](https://wiki.osdev.org/GCC_Cross-Compiler))

## How to Use
1. Build the OS.

    ```make os```

2. Run Jasos in your VM of choice. I use QEMU. (Don't run this on actual hardware you madman).

    ```qemu-system-i386 -kernel bin/jasos.bin```

Alternatively, you can do it all in one shot with `make run`.

## Credits
Credit to the OSDev community for the [Bare Bones Tutorial](https://wiki.osdev.org/Bare_Bones) which the multiboot implementation and linker script come from.