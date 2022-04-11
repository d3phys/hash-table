#include <htab.h>
#include <string.h>


hash_t crc32_hash(hkey key) 
{
        unsigned int byte = 0, mask = 0;
        unsigned int crc = 0xFFFFFFFF;
        
        for (int i = 0; key[i] != 0; i++) {
                byte = key[i];
                crc = crc ^ byte;
                
                for (int j = 7; j >= 0; j--) {
                        mask = -(crc & 1);
                        crc = (crc >> 1) ^ (0xEDB88320 & mask);
                }
        }

        return (hash_t)(~crc);
}

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

hash_t wsum_ascii_hash(hkey key)
{
        assert(key);

        hash_t hash = 0;
        size_t i = 1;        
        while (*key)
                hash += *key++ * i++;
                
        return hash;
}
