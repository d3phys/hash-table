#include <assert.h>
#include <errno.h>
#include <string.h>
#include <htab.h>
#include <logs.h>
#include <entry.h>

int compare_hrecs(hrec rec1, hrec rec2)
{
        size_t len1 = strlen(rec1.key);
        size_t len2 = strlen(rec2.key);
        
        unsigned min = len1 > len2 ? len2 : len1; 
        return strncmp(rec1.key, rec2.key, min);
}

htab *htab_ctor(htab *const ht, hash_t (* hfunc)(hkey), const size_t init_cap)
{
        assert(ht);      
        assert(hfunc);  
        int saved_errno = 0;

        const hash_t n_recs = (hash_t)(-1);
        fprintf(stderr, "CHAR: %d\n", (int)n_recs);
        
        list *slots = (list *)calloc(n_recs, sizeof(list));
        if (!slots) {
                plogs("Hash table slots allocation fail: %s\n", strerror(errno));
                return nullptr;
        }

        for (hash_t i = 0; i < n_recs; i++) {
                list *lst = construct_list(slots + i, init_cap);
                if (!lst) {
                        saved_errno = errno;
                        plogs("Hash list %d allocation fail: %s\n", i, strerror(errno));

                        for (; i > 0; i--)
                                destruct_list(slots + i);
                                
                        free(slots);
                        
                        errno = saved_errno;
                        return nullptr;
                }
        }

        ht->slots = slots;
        ht->hfunc = hfunc;
        return ht;
}

htab *htab_dtor(htab *const ht)
{
        assert(ht);

        list *slots = ht->slots;
        for (hash_t i = 0; i < (hash_t)(-1); i++)
                destruct_list(slots + i);
                
        free(slots);

        ht->slots = nullptr;
        ht->hfunc = nullptr;
        return ht;
}

hrec *htab_insert(htab *const ht, hrec rec)
{
        assert(ht);

        hash_t indx = ht->hfunc(rec.key);
        ptrdiff_t ins = list_insert_back(ht->slots + indx, rec);
        if (!ins) {
                plogs("Hash list insertion failed\n");
                return nullptr;
        }

        return &ht->slots[indx].nodes[ins].data;
}

hrec *htab_find(htab *const ht, hkey key, hash_t *slot)
{
        assert(ht);
        assert(key);
        
        hash_t indx = ht->hfunc(key);
        ptrdiff_t found = list_find(ht->slots + indx, {key, (hval) nullptr});
        if (!found)
                return nullptr;
                
        *slot = indx;       
        return &ht->slots[indx].nodes[found].data;
}

hrec *htab_delete(htab *const ht, hkey key)
{
        assert(ht);
        assert(key);
        
        hash_t indx = ht->hfunc(key);
        
        ptrdiff_t found = list_find(ht->slots + indx, {key, (hval) nullptr});
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
        if (item.key != (hkey)0xDEAD)
                fprintf(file, "data: %s", item.key); 
        else
                fprintf(file, "data: %p", item.key);    
}
