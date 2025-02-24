#include "inputmanager.h"

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MIN_ARGS            = 1;
const int MAX_ARGS            = 3;
const int Q_ARG               = 2;
const int CACHE_ARG           = 1;

const char* QUIET_COMMAND     = "-q";
const char* NULL_CACHE        = "null";

const size_t BUFFER_SIZE      = 100;

const char* INPUT_LIMIT       = "9223372036854775807";
const size_t INPUT_MAX_DIGITS = 19;

int getInput(char* buffer, bool hide_prompt) {
    memset(buffer, 0, BUFFER_SIZE);

    if (!hide_prompt)
        printf("Enter money in cents (EOF to exit): ");
    fgets(buffer, BUFFER_SIZE, stdin);

    int exit_code = USER_CONTINUE;

    if (feof(stdin))
        exit_code = USER_EXIT;

    // clear buffer if full
    if (strnlen(buffer, BUFFER_SIZE) == BUFFER_SIZE - 1)
        clearInputBuffer();

    return exit_code;
}

int validateQuietCommand(int argc, char* argv[]) {
    if (argc <= Q_ARG)
        return NO_Q_ARG;

    if (strncmp(argv[Q_ARG], QUIET_COMMAND, BUFFER_SIZE) != 0)
        return BAD_Q_ARG;

    return HAS_Q_ARG;
}

bool canLoadCache(int argc, char* argv[]) {
    return argc > CACHE_ARG &&
           strncmp(argv[CACHE_ARG], NULL_CACHE, BUFFER_SIZE) != 0;
}

int validateInput(const char* input) {
    size_t nonzeros    = 0;
    size_t ix          = 0;
    size_t offset      = 0;
    bool leading_zeros = false;

    // Count length, ignoring negative sign and leading 0's
    while (input[ix] != '\n' && input[ix] != '\0') {
        char ch = input[ix];

        // Offset to ignore negative sign
        if (ch == '-' && ix == 0) {
            offset++;

            // Offset to ignore leading 0's
        } else if (ch == '0' && nonzeros == 0) {
            offset++;
            leading_zeros = true;

        } else if (!isdigit(ch)) {
            return INPUT_INVALID;

        } else {
            nonzeros++;
        }
        ix++;
    }
    // Allows '0' and '-0' to be valid, but not '-' or empty strings
    if (nonzeros == 0)
        return (leading_zeros) ? INPUT_OK : INPUT_INVALID;

    if (compareInput(input, nonzeros, offset) > 0)
        return INPUT_OUT_OF_RANGE;

    return INPUT_OK;
}

int compareInput(const char* input, size_t length, size_t offset) {
    if (length > INPUT_MAX_DIGITS)
        return 1;
    if (length < INPUT_MAX_DIGITS)
        return -1;

    return strncmp(input + offset, INPUT_LIMIT, INPUT_MAX_DIGITS);
}

void clearInputBuffer() {
    char ch = ' ';
    while (ch != '\n' && ch != EOF)
        ch = getchar();
}

void printError(int errcode, const char* input, bool descriptive) {
    char attempted[BUFFER_SIZE];

    // Copy input to attempted and remove newline at the end
    strncpy(attempted, input, BUFFER_SIZE - 1);
    attempted[BUFFER_SIZE - 1]          = '\0';
    attempted[strcspn(attempted, "\n")] = '\0';

    if (descriptive) {
        switch (errcode) {
            case INPUT_OK:
                return;

            case INPUT_INVALID:
                printf(
                    "Invalid input: '%s'\n"
                    "Please enter a 64-bit signed integer without whitespace "
                    "or other symbols.\n",
                    attempted);
                return;

            case INPUT_OUT_OF_RANGE:
                printf(
                    "'%s' is out of range.\nNumber must be within 64 bits. "
                    "(-%s <= {num} <= %s)\n",
                    attempted, INPUT_LIMIT, INPUT_LIMIT);
                return;

            case CONVERT_ERROR:
                printf("Error converting '%s' to words\n", attempted);
                return;

            case BAD_ARG_COUNT:
                printf(
                    "Usage: %s [cache.so | %s] [%s]\n"
                    "%s: Run without loading a cache\n"
                    "%s: Hide prompt and error code details\n",
                    attempted, NULL_CACHE, QUIET_COMMAND, NULL_CACHE,
                    QUIET_COMMAND);
                return;

            case BAD_Q_ARG:
                printf("Unknown flag: '%s'\n", attempted);
                return;

            case BAD_CACHE_ARG:
                printf("Could not load cache module '%s'\n", attempted);
                return;
        }
    }
    printf("Received error code %d with an input of '%s'\n", errcode,
           attempted);
}
