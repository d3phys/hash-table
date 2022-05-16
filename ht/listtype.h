#ifndef ENTRY_H
#define ENTRY_H

#include <immintrin.h>

typedef __m256i hkey;

typedef size_t hval; 

typedef struct hrec {
        hkey key;
        hval val = 0;
} item_t;

const item_t FREE_DATA = {
        .val = (hval)0xDEAD,
};

const item_t INIT_DATA = {
        .val = 0,
};

#endif /* ENTRY_H */
