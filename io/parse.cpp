#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <parse.h>
#include <logs.h>

const char **parse_words(char *const str, size_t *count)
{
        assert(str);
        errno = 0;
        
        size_t n_words = 0;
        for (const char *cur = str; *cur; n_words++) {
                while (isspace(*cur) || ispunct(*cur))
                        cur++;

                if (!*cur) break;
                
                while (*cur && !isspace(*cur) && !ispunct(*cur))
                        cur++;
        }

        if (!n_words) {
                fprintf(stderr, "There are no words inside\n");
                return nullptr;
        }
                
        const char **words = (const char **)calloc(n_words, sizeof(const char *));
        if (!words) {
                fprintf(stderr, "Words allocation failed: %s\n", strerror(errno));
                return nullptr;
        }

        const char **w = words; 
        for (char *cur = str; *cur; w++) {
                while (isspace(*cur) || ispunct(*cur))
                        cur++;

                if (!*cur) break;
                
                *w = cur;
                while (*cur && !isspace(*cur) && !ispunct(*cur))
                        cur++;

                if (*cur)
                        *cur++ = '\0';
        }        

        *count = n_words;
        return words;
}
