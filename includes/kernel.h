#ifndef KERNEL_H
#define KERNEL_H

#include <stdbool.h>

void system(const char *command);
void assert(bool condition, const char *message);

#endif
