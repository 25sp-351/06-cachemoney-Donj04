#include "cache.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "converter.h"

#define CACHE_SIZE 1500
#define STR_LENGTH 256

// typedef struct cache {
//     char values[CACHE_SIZE][STR_LENGTH];
// } *Cache;

Cache newCache() {
    Cache new_cache = malloc(sizeof(struct cache));
    for (size_t ix = 0; ix < CACHE_SIZE; ix++)
        memset(new_cache->values[ix], '\0', STR_LENGTH);
    return new_cache;
}

void freeCache(Cache cache) {
    if (cache != NULL)
        free(cache);
}

bool validCacheIdx(const Cache cache, long long num) {
    return (cache != NULL) && (num >= 0) && (num < CACHE_SIZE);
}

char* getCached(Cache cache, long long num) {
    if (validCacheIdx(cache, num)) {
        // printf("received '%s' from idx %lld\n", cache->values[num], num);
        return cache->values[num];
    }
    return NULL;
}

bool numIsCached(Cache cache, long long num) {
    // printf("looking in cache idx %lld... ", num);
    if (validCacheIdx(cache, num)) {
        char* str = getCached(cache, num);
        if (str != NULL && str[0] != '\0') {
            // printf(" it is cached\n");
            return true;
        }
    }
    // printf(" not cached\n");
    return false;
}

void setCacheVal(Cache cache, long long num, const char* new_str) {
    if (validCacheIdx(cache, num)) {
        char* curr = getCached(cache, num);
        strncpy(curr, new_str, STR_LENGTH - 1);
        curr[STR_LENGTH - 1] = '\0';
        // printf("added %lld: '%s' to cache\n", num, curr);
    }
}

void addToCache(Cache cache, long long num, const char* new_str) {
    if (!numIsCached(cache, num))
        setCacheVal(cache, num, new_str);
    // else
    //     printf("%lld is already cached\n", num);
}
