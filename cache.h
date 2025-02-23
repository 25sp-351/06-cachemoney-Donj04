#ifndef CACHE_H
#define CACHE_H

#include <stdbool.h>

#define CACHE_SIZE 1500
#define STR_LENGTH 256

typedef struct cache {
    char values[CACHE_SIZE][STR_LENGTH];
} *Cache;

Cache newCache();

void freeCache(Cache cache);

// Check if cache exists and number is in bounds
bool validCacheIdx(const Cache cache, long long num);

char* getCached(Cache cache, long long num);

bool numIsCached(Cache cache, long long num);

void setCacheVal(Cache cache, long long num, const char* new_str);

// Writes to cache only if this number is not already cached
void addToCache(Cache cache, long long num, const char* new_str);

#endif
