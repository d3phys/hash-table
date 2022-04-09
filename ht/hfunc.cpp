#include <htab.h>

hash_t one_hash(hkey key)
{
        assert(key);
        return (hash_t)1; 
}

hash_t sum_ascii_hash(hkey key)
{
        assert(key);
        return (hash_t)0;
}

hash_t first_ascii_hash(hkey key)
{
        assert(key);
        return *(hash_t *)key;       
}
