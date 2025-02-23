#include "inputmanager.h"

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* QUIET_COMMAND     = "-q";

const size_t BUFFER_SIZE      = 100;

const char* LLONG_MAX_STR     = "9223372036854775807";
const size_t LLONG_MAX_DIGITS = 19;

int inputInRange(const char* input, size_t length, size_t offset) {
    if (length == 0)
        return INPUT_INVALID;
    if (length > LLONG_MAX_DIGITS)
        return INPUT_OUT_OF_RANGE;
    if (length < LLONG_MAX_DIGITS)
        return INPUT_OK;

    // Compares the most significant non-equal pair of digits.
    // Returns true if input's digit is less, false if greater.
    // If they are both equal, input is still in range
    for (size_t iy = 0; iy < LLONG_MAX_DIGITS; iy++) {
        const char inp_c   = input[iy + offset];
        const char limit_c = LLONG_MAX_STR[iy];

        if (inp_c < limit_c)
            return INPUT_OK;
        if (inp_c > limit_c)
            return INPUT_OUT_OF_RANGE;
    }
    return INPUT_OK;
}

int inputValid(const char* input) {
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
    // Allows '0' and '-0' to be valid, but not '-'
    if (nonzeros == 0) {
        if (leading_zeros)
            return INPUT_OK;
        return INPUT_INVALID;
    }
    return inputInRange(input, nonzeros, offset);
}

void printError(int errcode, const char* input, int argc, char* argv[]) {
    char attempted[BUFFER_SIZE];
    bool hide_extra = hasQuietCommand(argc, argv) == HAS_Q_ARG;

    // Copy input to attempted and remove newline at the end
    strncpy(attempted, input, BUFFER_SIZE - 1);
    attempted[BUFFER_SIZE - 1]          = '\0';
    attempted[strcspn(attempted, "\n")] = '\0';

    if (!hide_extra) {
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
                    attempted, LLONG_MAX_STR, LLONG_MAX_STR);
                return;

            case CONVERT_ERROR:
                printf("Error converting '%s' to words\n", attempted);
                return;

            case USER_BAD_ARG:
                printf(
                    "Usage: %s [%s]\n"
                    "%s: Hide prompt\n",
                    argv[0], QUIET_COMMAND, QUIET_COMMAND);
                return;
        }
    }
    printf("Received error code %d with an input of '%s'\n", errcode,
           attempted);
}

void clearInputBuffer() {
    char ch = ' ';
    while (ch != '\n' && ch != EOF)
        ch = getchar();
}

int hasQuietCommand(int argc, char* argv[]) {
    if (argc != 1) {
        if (argc > 2)
            return USER_BAD_ARG;
        if (strncmp(argv[1], "-q", 3) != 0)
            return USER_BAD_ARG;
        return HAS_Q_ARG;
    }
    return NO_Q_ARG;
}

int getInput(int argc, char* argv[], char* buffer) {
    int exit_code    = USER_CONTINUE;
    bool hide_prompt = true;
    int q_state      = hasQuietCommand(argc, argv);

    if (q_state != HAS_Q_ARG && q_state != NO_Q_ARG)
        return q_state;
    hide_prompt = q_state == HAS_Q_ARG;

    memset(buffer, 0, BUFFER_SIZE);

    if (!hide_prompt)
        printf("Enter money in cents (EOF to exit): ");
    fgets(buffer, BUFFER_SIZE, stdin);

    if (feof(stdin))
        exit_code = USER_EXIT;

    // printf("buffer size: %ld\n", strnlen(buffer, BUFFER_SIZE));
    if (strnlen(buffer, BUFFER_SIZE) == BUFFER_SIZE - 1)
        clearInputBuffer();

    return exit_code;
}
