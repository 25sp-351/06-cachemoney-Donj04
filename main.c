#include <stdio.h>

#include "cache.h"
#include "converter.h"
#include "inputmanager.h"

int main(int argc, char* argv[]) {
    if (argc < MIN_ARGS || argc > MAX_ARGS) {
        printError(BAD_ARG_COUNT, argv[0], true);
        return 1;
    }

    int q_state = validateQuietCommand(argc, argv);
    bool quiet_on;

    if (q_state == HAS_Q_ARG)
        quiet_on = true;
    else if (q_state == NO_Q_ARG)
        quiet_on = false;
    else {
        printError(BAD_Q_ARG, argv[Q_ARG], true);
        return 1;
    }

    ProviderFunction provider = moneyToStr;

    bool cache_installed      = canLoadCache(argc, argv);
    Cache* cache              = NULL;

    if (cache_installed) {
        cache = load_cache_module(argv[CACHE_ARG]);

        if (cache == NULL)
            return 1;

        provider                  = cache->set_provider_func(provider);
        *(cache->show_debug_info) = false;
        if (!quiet_on)
            printf("Cache loaded\n\n");
    }

    char input[BUFFER_SIZE];

    while (true) {
        int user_state = getInput(input, quiet_on);

        if (user_state == USER_EXIT) {
            // Makes sure the command line is on the next line after previous
            // outputs
            printf("\n");
            break;

        } else if (user_state == USER_CONTINUE) {
            int input_state = validateInput(input);

            if (input_state != INPUT_OK) {
                printError(input_state, input, !quiet_on);
            } else {
                long long input_num;
                sscanf(input, "%lld", &input_num);

                char* str = provider(input_num);

                printf("%s\n", str);
            }
        } else {
            printError(user_state, input, !quiet_on);
        }
    }

    if (cache_installed && !quiet_on) {
        printf("\n\n");

        CacheStat* list_of_stats = cache->get_statistics();
        print_cache_stats(fileno(stdout), list_of_stats);

        if (list_of_stats)
            free(list_of_stats);

        printf("\n\n");

        cache->cache_cleanup();
        free(cache);
    }

    return 0;
}
