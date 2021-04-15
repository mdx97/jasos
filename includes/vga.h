#ifndef VGA_H
#define VGA_H

void vga_init();
void vga_put(int x, int y, char c);
void vga_write(int x, int y, char* string);

#endif
