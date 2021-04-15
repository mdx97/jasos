#ifndef SYSCALLS_H
#define SYSCALLS_H

void syscall_put(int x, int y, char c);
void syscall_write(int x, int y, char* string);
char* syscall_version();

#endif
