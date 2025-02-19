#ifndef _STDIO_H
#define _STDIO_H

#include "stdarg.h"

extern int vprintf(const char *format, va_list ap);
extern int vsprintf(char *str, const char *format, va_list ap);
extern int vsnprintf(char *str, size_t size, const char *format, va_list ap);
extern int vasprintf(char **ret, const char *format, va_list ap);

#endif
