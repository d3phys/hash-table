#include <assert.h>
#include <errno.h>
#include <string.h>
#include <htab.h>
#include <logs.h>
#include <listtype.h>


int compare_keys(hkey *k1, hkey *k2)
{
$$
        /*return ~_mm256_movemask_epi8(
                _mm256_cmpeq_epi64(_mm256_load_si256(k1), _mm256_load_si256(k2))
        );*/

        return ~_mm256_movemask_epi8(
                _mm256_cmpeq_epi64(_mm256_stream_load_si256(k1), _mm256_stream_load_si256(k2))
        );
        /*
        
        
        return memcmp(k1, k2, 32);
        //return !strncmp((char *)k1, (char *)k2, 32);
        /*return ~(_mm256_movemask_ps(
                _mm256_cmp_ps(
                        _mm256_castsi256_ps(_mm256_load_si256(k1)),
                        _mm256_castsi256_ps(_mm256_load_si256(k2)),
                        _CMP_EQ_OQ
                )
        ));*/
}

htab *htab_ctor(htab *const ht, hash_t (* hfunc)(hkey *), const size_t init_cap)
{
        assert(ht);      
        assert(hfunc);  
        int saved_errno = 0;
$$
        fprintf(stderr, "CHAR: %ld\n", (hash_t)(-1) + 1);
        
        list *slots = (list *)calloc((hash_t)(-1) + 1, sizeof(list));
        if (!slots) {
                plogs("Hash table slots allocation fail: %s\n", strerror(errno));
                return nullptr;
        }
$$

        hash_t i = 0; 
        do {
                list *lst = construct_list(slots + i, init_cap);
$$

                if (!lst) {
$$
                        saved_errno = errno;
                        plogs("Hash list %d allocation fail: %s\n", i, strerror(errno));

                        while (i--)
                                destruct_list(slots + i);
                                
                        free(slots);
                        
                        errno = saved_errno;
                        return nullptr;
                }                
        } while (++i);
$$

        ht->slots = slots;
        ht->hfunc = hfunc;
        return ht;
}

htab *htab_dtor(htab *const ht)
{
        assert(ht);

        list *slots = ht->slots;

        hash_t i = 0;
        do { 
                destruct_list(slots + i); 
        } while (++i);
        
        free(slots);

        ht->slots = nullptr;
        ht->hfunc = nullptr;
        return ht;
}

hrec *htab_insert(htab *const ht, hrec *rec)
{
        assert(ht);
$$
        hash_t indx = ht->hfunc(&rec->key);
        plogs("hash: %d\n", indx);
        $$
        ptrdiff_t ins = list_insert_back(ht->slots + indx, rec);
        $$
        if (!ins) {
                plogs("Hash list insertion failed\n");
                return nullptr;
        }

        return &ht->slots[indx].nodes[ins].data;
}

ptrdiff_t htab_list_find(list *const lst, hkey *key)
{
        assert(key);
        assert(lst);
        
        ptrdiff_t cur = lst->head;
        while (cur && compare_keys(&lst->nodes[cur].data.key, key))
                cur = lst->nodes[cur].next;    
                
        return cur;
}

hrec *htab_find(htab *const ht, hkey *key, hash_t *slot)
{
        assert(ht);
        assert(key);
        
        hash_t indx = ht->hfunc(key);

        ptrdiff_t found = htab_list_find(ht->slots + indx, key);
        if (!found)
                return nullptr;

        if (slot)     
                *slot = indx;
                       
        return &ht->slots[indx].nodes[found].data;
}

hrec *htab_delete(htab *const ht, hkey *key)
{
        assert(ht);
        assert(key);
        
        hash_t indx = ht->hfunc(key);

        ptrdiff_t found = htab_list_find(ht->slots + indx, key);
        if (!found)
                return nullptr;

        list_delete(ht->slots + indx, found);                   
        return &ht->slots[indx].nodes[found].data;
}

void htab_dump(htab *const ht, hash_t from, hash_t count)
{
        assert(ht);
        assert(count);

        if ((hash_t)(from + count) < from) {
                fprintf(logs, html(RED, "Dump overflow"));
                return;
        }
        
        fprintf(logs, "-----------------------------------------\n");
        fprintf(logs, html(BLUE, "Hash table dump (slots %u:%u) \n"), from, from + count - 1);
        fprintf(logs, "-----------------------------------------\n");

        for (hash_t i = from; i < from + count; i++) {
                fprintf(logs, html(GREEN, "slot %3.d:"), i);

                node   *nodes = ht->slots[i].nodes;
                ptrdiff_t cur = ht->slots[i].head; 
                
                while (cur) {
                        fprintf(logs, " %s ", nodes[cur].data.key);
                        cur = nodes[cur].next;
                }
                
                fprintf(logs, "\n");
        }
        
        fprintf(logs, "-----------------------------------------\n");
}

void list_print_item(FILE *file, item_t item)
{
        if (item.val != (hval)0xDEAD)
                fprintf(file, "data: %s", item.key); 
        else
                fprintf(file, "data: %p", item.key);    
}
