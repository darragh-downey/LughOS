#ifndef CONSOLE_H
#define CONSOLE_H

#include <stddef.h>
#include "lugh.h"

// Use log_level_t from lugh.h

// console interface functions
void console_init(void);
void console_putchar(char c);
void console_write(const char *str, size_t len);

#endif // CONSOLE_H