#ifndef SHELL_H
#define SHELL_H

void shell_init();
void shell_clear();
void shell_input(char c);
void shell_output(const char *string);
void shell_ready_input();
void shell_putchar(char c);

#endif
