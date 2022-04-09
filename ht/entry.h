#ifndef ENTRY_H
#define ENTRY_H

typedef const char * hkey;

typedef double *hval; 

typedef struct hrec {
        hkey key = nullptr;
        hval val = nullptr;
} item_t;

const item_t FREE_DATA = {
        .key = (hkey)0xDEAD,
        .val = (hval)0xDEAD
};

const item_t INIT_DATA = {
        .key = nullptr,
        .val = nullptr
};

int compare_hrecs(hrec rec1, hrec rec2);
int (*const list_compare)(item_t itm1, item_t itm2) = &compare_hrecs;

#endif /* ENTRY_H */
