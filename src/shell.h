#ifndef SHELL_H
#define SHELL_H

void shell_init();
void shell_clear();
void shell_input(char c);
void shell_output(const char *string);
void shell_output_line(const char *string);
void shell_linebreak();
void shell_ready_input();
        
#endif
