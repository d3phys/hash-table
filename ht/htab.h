#ifndef HTABLE_H
#define HTABLE_H

#include <list.h>

typedef unsigned char hash_t;

extern "C" hash_t crc32_sse(hkey *key);

hash_t         one_hash(hkey *key);
hash_t         ror_hash(hkey *key);
hash_t       crc32_hash(hkey *key);
hash_t      length_hash(hkey *key);
hash_t   sum_ascii_hash(hkey *key);
hash_t  wsum_ascii_hash(hkey *key);
hash_t first_ascii_hash(hkey *key);

struct htab {
        list *slots              = nullptr;
        hash_t (* hfunc)(hkey *) = nullptr;
};

htab *htab_ctor(htab *const ht, hash_t (* hfunc)(hkey *), const size_t init_cap = 1);
htab *htab_dtor(htab *const ht);

hrec *htab_insert(htab *const ht, hrec *rec);
hrec *htab_delete(htab *const ht, hkey *key);
hrec *htab_find(htab *const ht, hkey *key, hash_t *slot = nullptr);

void htab_dump(htab *const ht, hash_t from = 0, hash_t count = (hash_t)-1);

#endif /* HTABLE_H */
