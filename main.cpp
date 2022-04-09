#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <list.h>
#include <logs.h>
#include <entry.h>
#include <htab.h>
#include <iommap.h>
#include <parse.h>

static int analyze_hash(
        FILE *file, 
        size_t *const freq, 
        const char **words, 
        size_t n_words, 
        hash_t (* hfunc)(hkey), 
        const char *label
);

int main(int argc, const char *argv[])
{
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

        size_t *freq = (size_t *)calloc((hash_t)-1, sizeof(size_t));
        if (!freq) {
                fprintf(stderr, "Frequency table allocation failed: %s\n", strerror(errno));
                fclose(dest);
                free(words);
                mmap_free(&md);
                return EXIT_FAILURE;
        }

        analyze_hash(dest, freq, words, n_words, &first_ascii_hash, "first_ascii_hash");
        analyze_hash(dest, freq, words, n_words,         &one_hash,         "one_hash");

        fclose(dest);
        free(freq);
        free(words);
        mmap_free(&md);
         
        return EXIT_SUCCESS;
}

static int analyze_hash(
        FILE *file, 
        size_t *const freq, 
        const char **words, 
        size_t n_words, 
        hash_t (* hfunc)(hkey), 
        const char *label
){
        memset(freq, 0, sizeof(size_t) * (hash_t)-1);

        htab ht = {};
        if (!htab_ctor(&ht, hfunc))
                return EXIT_FAILURE;       
        
        for (size_t w = 0; w < n_words; w++)
                htab_insert(&ht, {words[w], nullptr});

        hash_t slot = 0;
        for (size_t w = 0; w < n_words; w++) {
                if (!htab_find(&ht, words[w], &slot)) {
                        fprintf(stderr, "NOT FOUND\n");
                        continue;
                }
                        
                freq[slot]++;                
        }

        fprintf(file, "%s", label);
        for (hash_t s = 0; s < (hash_t)-1; s++)
                fprintf(file, " %lu", freq[s]);

        fprintf(file, "\n");    
        htab_dtor(&ht);
        
        return EXIT_SUCCESS;
}
