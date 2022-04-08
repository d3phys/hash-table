#ifndef LOGS_H
#define LOGS_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

extern FILE *logs;

char *local_time(const char *const fmt);


#ifdef LOGS_MEMORY

void *__logs_calloc(size_t num, size_t size, unsigned line, const char *func, const char *file);
void *__logs_realloc(void *ptr, size_t size, unsigned line, const char *func, const char *file);
void  __logs_free(void *ptr, unsigned line, const char *func, const char *file);

#define realloc(ptr, size) __logs_realloc(ptr, size, __LINE__, __PRETTY_FUNCTION__, __FILE__)
#define calloc(num, size)  __logs_calloc (num, size, __LINE__, __PRETTY_FUNCTION__, __FILE__)
#define free(ptr)          __logs_free         (ptr, __LINE__, __PRETTY_FUNCTION__, __FILE__)

#endif /* LOGS_MEMORY */


#ifdef LOGS_COLORS

enum ascii_colors {
        ASCII_RED    = 1,
        ASCII_GREEN  = 2,
        ASCII_YELLOW = 3,
        ASCII_BLUE   = 4,
        ASCII_WHITE  = 7,
};

#define ascii(color, fmt) "\u001b[3%dm" fmt "\u001b[0m", ASCII_##color

#define html(color, fmt) "<font color=\"" #color "\">" fmt "</font>"

#define bold(fmt)   "<b>" fmt "</b>"
#define italic(fmt) "<i>" fmt "</i>"

#define $(code) fprintf(logs, "%s: %s\n", __PRETTY_FUNCTION__, #code); code

#define $$ fprintf(logs, "%s: %d\n", __PRETTY_FUNCTION__, __LINE__);

#endif /* LOGS_COLORS */


#endif /* LOGS_H */
