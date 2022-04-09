#include <htab.h>
#include <string.h>

hash_t one_hash(hkey key)
{
        assert(key);
        return (hash_t)1; 
}

hash_t length_hash(hkey key)
{
        assert(key);
        return (hash_t)strlen(key);
}

hash_t first_ascii_hash(hkey key)
{
        assert(key);
        return *(hash_t *)key;       
}

hash_t sum_ascii_hash(hkey key)
{
        assert(key);

        hash_t hash = 0;
        while (*key)
                hash += *key++;
                
        return hash;
}

hash_t ror_hash(hkey key)
{
        assert(key);

        hash_t hash = 0;
        while (*key)
                hash = (hash_t)((hash >> 1) | (hash << 7)) ^ *(hash_t *)key++;
                
        return hash;
}
