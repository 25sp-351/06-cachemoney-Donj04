#include <bsd/stdio.h>
#include <bsd/stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache.h"
#include "converter.h"

/*
** This is a cache tester for NON-RECURSIVE functions.
** It does not support the provider function accessing the cache.
*/

#define TEST_COUNT 3000
#define MAX_TEST_NUMBER 1000
#define MIN_TEST_NUMBER -100

int rand_between(int min, int max);

const char *get_num_suffix(int num);

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [cache.so]\n", argv[0]);
        return 1;
    }

    // base (real) function
    ProviderFunction get_me_a_value = moneyToStr;

    bool cache_installed            = argc > 1;
    Cache *cache                    = NULL;

    if (cache_installed) {
        cache = load_cache_module(argv[1]);

        if (cache == NULL) {
            fprintf(stderr, "Failed to load cache module\n");
            return 1;
        }
        // replace our real provider with a caching provider
        get_me_a_value = cache->set_provider_func(get_me_a_value);
        *(cache->show_debug_info) = true;
    }

    // do yo tests using test_me like:
    for (int test_number = 0; test_number < TEST_COUNT; test_number++) {
        int randomnumber = rand_between(MIN_TEST_NUMBER, MAX_TEST_NUMBER);

        printf("\n\nBeginning test %2d: %d\n", test_number, randomnumber);

        ValueType result = get_me_a_value(randomnumber);

        printf("Done with test %2d: " VALUE_FMT "\n", test_number, result);

        // if (cache != NULL && test_number == TEST_COUNT / 2) {
        //     printf("Taking a break. Resetting cache statistics.\n");
        //     cache->reset_statistics();
        // }
    }

    if (cache_installed) {
        printf("\n\n");

        CacheStat *list_of_stats = cache->get_statistics();
        print_cache_stats(fileno(stdout), list_of_stats);

        if (list_of_stats)
            free(list_of_stats);

        printf("\n\n");

        cache->cache_cleanup();
        free(cache);
    }
}

int rand_between(int min, int max) {
    int range = max - min;
    return min + arc4random_uniform(range);
}
