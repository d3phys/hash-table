#ifndef ENTRY_H
#define ENTRY_H

typedef char hkey[32];

typedef size_t hval; 

typedef struct hrec {
        hkey key = {0};
        hval val = 0;
} item_t;

const item_t FREE_DATA = {
        .key = {0},
        .val = (hval)0xDEAD,
};

const item_t INIT_DATA = {
        .key = {0},
        .val = 0,
};

#endif /* ENTRY_H */
