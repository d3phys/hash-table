#include <htab.h>
#include <string.h>
#include <immintrin.h>
#include <logs.h>

hash_t crc32_hash(hkey *key) 
{
        int hash = 0x04C11DB7;
$$

//        fprintf(logs, "hkey address: %p %b\n", key, key);        
        int *ik = (int *)key;
        for (size_t i = 0; i < sizeof(hkey)/sizeof(int); i++)
                hash = _mm_crc32_u32(hash, *ik++);        
        $$
        return (hash_t)(hash);
}

/*
hash_t crc32_hash(hkey *key) 
{
        unsigned int byte = 0, mask = 0;
        unsigned int crc = 0xFFFFFFFF;
        
        for (int i = 0; (*key)[i] != 0; i++) {
                byte = (*key)[i];
                crc = crc ^ byte;
                
                for (int j = 7; j >= 0; j--) {
                        mask = -(crc & 1);
                        crc = (crc >> 1) ^ (0xEDB88320 & mask);
                }
        }

        return (hash_t)(~crc);
}*/

/*
hash_t one_hash(hkey *key)
{
        assert(key);
        return (hash_t)1; 
}

hash_t length_hash(hkey *key)
{
        assert(key);
        return (hash_t)strlen(*key);
}

hash_t first_ascii_hash(hkey *key)
{
        assert(key);
        return *(hash_t *)(*key);       
}

hash_t sum_ascii_hash(hkey *key)
{
        assert(key);

        hash_t hash = 0;
        char *cur = *key;
        while (*cur)
                hash += *cur++;
                
        return hash;
}

hash_t ror_hash(hkey *key)
{
        assert(key);
        
        char *cur = *key;
        hash_t hash = 0;
        while (*cur)
                hash = (hash_t)((hash >> 1) | (hash << 7)) ^ *(hash_t *)cur++;
                
        return hash;
}

hash_t wsum_ascii_hash(hkey *key)
{
        assert(key);

        char *cur = *key;
        size_t i = 1;
                
        hash_t hash = 0;
        while (*cur)
                hash += *cur++ * i++;
                
        return hash;
}*/
