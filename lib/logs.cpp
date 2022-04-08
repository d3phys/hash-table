#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

FILE *logs = nullptr;


#ifdef LOGS_FILE

__attribute__((constructor(101)))
static void __logs_init()
{ 
        logs = fopen(LOGS_FILE, "w");
        if (logs) {
                int err = setvbuf(logs, nullptr, _IONBF, 0);
                if (!err) {
                        fprintf(logs, "<pre>\n");
                        return;
                }

                fclose(logs);
        }

        logs = stderr;
}

__attribute__((destructor))
static void __logs_die()
{
        fclose(logs);
}

#else

__attribute__((constructor(101)))
static void __logs_init() { logs = stderr; }

#endif /* LOGS_FILE */ 

       
/*
 * Function uses local static buffer with a constant length.
 * It is designed to be called multiple times.
 *
 * Returns local time as formatted string.
 */
char *local_time(const char *const fmt)
{
        assert(fmt);

        static const size_t buf_size = 0xFF;
        static char str_tm[buf_size] = {0};

        static time_t t = time(nullptr);
        if (t == -1)
                return nullptr;

        static tm *lt = localtime(&t);
        strftime(str_tm, buf_size, fmt, lt);

        return str_tm;
}

#ifdef LOGS_MEMORY

void *__logs_calloc(size_t num, size_t size, unsigned line, const char *func, const char *file);
void *__logs_realloc(void *ptr, size_t size, unsigned line, const char *func, const char *file);
void  __logs_free(void *ptr, unsigned line, const char *func, const char *file);

void *__logs_calloc(size_t num, size_t size, unsigned line, const char *func, const char *file)
{
        assert(size);
        assert(num);
        
        void *ptr = calloc(num, size);
        fprintf(logs, "%s:%u:%s: calloc(%p, %lu) total %lu bytes\n", file, line, func, ptr, size, size * num);

        return ptr;
}

void *__logs_realloc(void *ptr, size_t size, unsigned line, const char *func, const char *file)
{
        assert(ptr);
        assert(size);
        
        ptr = realloc(ptr, size);
        fprintf(logs, "%s:%u:%s: realloc(%p, %lu) total %lu bytes\n", file, line, func, ptr, size, size);

        return ptr;
}

void __logs_free(void *ptr, unsigned line, const char *func, const char *file)
{
        assert(ptr); 
        
        fprintf(logs, "%s:%u:%s: free(%p)\n", file, line, func, ptr);
        free(ptr); 
}

#endif /* LOGS_MEMORY */
