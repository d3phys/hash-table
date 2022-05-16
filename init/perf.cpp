#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <list.h>
#include <listtype.h>
#include <htab.h>
#include <iommap.h>
#include <parse.h>
#include <logs.h>

static int analyze_hash(
        FILE *file, 
        size_t *const freq, 
        hkey *words, 
        size_t n_words, 
        hash_t (* hfunc)(hkey *), 
        const char *label
){
        memset(freq, 0, sizeof(size_t) * (hash_t)-1);
        $$

        htab ht = {};
        $$
        if (!htab_ctor(&ht, hfunc))
                return EXIT_FAILURE;       

        $$

        fprintf(stderr, "start\n");

        for (size_t i = 0; i < 100; i++)
        for (size_t w = 0; w < n_words; w++) {
                $$
                if (!htab_find(&ht, words + w)) {
                        $$
                        alignas(32) hrec ins = {};
                        memcpy((char *)&ins.key, words + w, 32); 
                        $$
                        htab_insert(&ht, &ins);
                }                
        }

        $$

        fprintf(stderr, "end\n");
        fprintf(file, "%s", label);

        hash_t s = 0;
        do {
                fprintf(file, ",%lu", ht.slots[s].n_nodes);
        } while (++s);

        fprintf(file, "\n");    
        htab_dtor(&ht);
        
        return EXIT_SUCCESS;
}

int main(int argc, const char *argv[])
{
        fprintf(stderr, "Hello\n");
        $$
        if (argc != 3) {
                fprintf(stderr, "Invalid arguments number\n");
                return EXIT_FAILURE;
        }

        FILE *dest = fopen(argv[2], "w");
        if (!dest) {
                fprintf(stderr, "Can't open destination file: %s\n", strerror(errno));
                return EXIT_FAILURE;
        }
        
        mmap_data md = {};
        int error = mmap_in(&md, argv[1]);
        if (error) {
                fclose(dest);
                return EXIT_FAILURE;
        }

        size_t n_words = 0;
        const char **words = parse_words(md.buf, &n_words);
        if (!words) {
                fclose(dest);
                mmap_free(&md);
                return EXIT_FAILURE;
        }

        hkey *keys = (hkey *)aligned_alloc(32, n_words * sizeof(hkey));
        memset(keys, 0, n_words * sizeof(hkey));
        for (size_t i = 0; i < n_words; i++)
                strncpy((char *)&keys[i], words[i], sizeof(hkey) - 1);
                
        size_t *freq = (size_t *)calloc((hash_t)(-1) + 1, sizeof(size_t));
        if (!freq) {
                fprintf(stderr, "Frequency table allocation failed: %s\n", strerror(errno));
                fclose(dest);
                free(words);
                mmap_free(&md);
                return EXIT_FAILURE;
        }
        
        fprintf(dest, "index");
        
        hash_t s = 0;
        do {
                fprintf(dest, ",%lu", s);
        } while (++s);
        
        fprintf(dest, "\n");
        analyze_hash(dest, freq, keys, n_words,  &crc32_sse,  "crc32_hash");

        fclose(dest);
        free(freq);
        free(words);
        free(keys);
        mmap_free(&md);
         
        return EXIT_SUCCESS;
}
