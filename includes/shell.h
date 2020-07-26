#ifndef SHELL_H
#define SHELL_H

extern const char *SHELL_INPUT_INDICATOR;

void shell_init();
void shell_clear();
void shell_input(char c);
void shell_write(const char *string);
void shell_writeline(const char *string);

#endif
