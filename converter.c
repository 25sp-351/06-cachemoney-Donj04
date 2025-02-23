#include "converter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inputmanager.h"

const long long QUADRILLION = 1000000000000000;
const long long TRILLION    = 1000000000000;
const long long BILLION     = 1000000000;
const long long MILLION     = 1000000;
const long long THOUSAND    = 1000;
const long long HUNDRED     = 100;

const char* ONES[]          = {" zero", " one", " two",   " three", " four",
                               " five", " six", " seven", " eight", " nine"};

const char* TEENS[]         = {" ten",      " eleven",  " twelve",  " thirteen",
                               " fourteen", " fifteen", " sixteen", " seventeen",
                               " eighteen", " nineteen"};

const char* TENS_MULT[]     = {"",        "",       " twenty", " thirty",
                               " forty",  " fifty", " sixty",  " seventy",
                               " eighty", " ninety"};

const char* POWERS_STR[]    = {" quadrillion", " trillion", " billion",
                               " million",     " thousand", " hundred"};

const long long POWERS_NUM[] = {QUADRILLION, TRILLION, BILLION,
                                MILLION,     THOUSAND, HUNDRED};

const size_t POWERS_COUNT    = 6;

void appendStr(char* dest, const char* src) {
    size_t length = strnlen(dest, STR_LENGTH);

    strncat(dest, src, STR_LENGTH - length - 1);
    dest[STR_LENGTH - 1] = '\0';
}

void clearStr(char* str) {
    memset(str, '\0', STR_LENGTH);
}

int printAsMoney(Cache cache, const char* input) {
    long long num;
    long long cents;

    if (sscanf(input, "%lld", &num) != 1)
        return CONVERT_ERROR;

    printf("%lld ->", num);

    if (num < 0) {
        num = -num;
        printf(" minus");
    }
    cents = num % 100;
    num /= 100;

    char dollars_str[STR_LENGTH];
    char cents_str[STR_LENGTH];

    convertNumToText(cache, num, dollars_str, true);
    convertNumToText(cache, cents, cents_str, true);

    printf("%s", dollars_str);
    printf(" %s and", (num == 1) ? "dollar" : "dollars");

    printf("%s", cents_str);
    printf(" %s\n", (cents == 1) ? "cent" : "cents");

    return CONVERT_SUCCESS;
}

void writeUpper(Cache cache, long long num, char* write_to) {
    // find biggest power that num is greater than or equal to
    // (hundred, thousand, million, etc)
    for (size_t ix = 0; ix < POWERS_COUNT; ix++) {
        if (num >= POWERS_NUM[ix]) {
            char* upper_str = malloc(STR_LENGTH);
            clearStr(upper_str);

            convertNumToText(cache, num / POWERS_NUM[ix], upper_str, false);
            appendStr(upper_str, POWERS_STR[ix]);
            convertNumToText(cache, num % POWERS_NUM[ix], upper_str, false);

            addToCache(cache, num, upper_str);
            appendStr(write_to, upper_str);

            free(upper_str);
            return;
        }
    }
}

void writeLower(Cache cache, long long num, char* write_to) {
    // only write numbers 0-99
    if (num >= HUNDRED)
        return;

    char* lower_str = malloc(STR_LENGTH);
    clearStr(lower_str);

    if (num >= 20) {  // append numbers 20-99
        appendStr(lower_str, TENS_MULT[num / 10]);
        if (num % 10 > 0)
            appendStr(lower_str, ONES[num % 10]);

    } else if (num >= 10) {  // append numbers 10-19
        appendStr(lower_str, TEENS[num - 10]);

    } else {  // append numbers 0-9
        appendStr(lower_str, ONES[num]);
    }
    addToCache(cache, num, lower_str);
    appendStr(write_to, lower_str);

    free(lower_str);
}

void convertNumToText(Cache cache, long long num, char* write_to, bool first) {
    if (num == 0 && !first)
        return;

    if (first)
        clearStr(write_to);

    if (numIsCached(cache, num)) {
        appendStr(write_to, getCached(cache, num));
        // printf("retrieved %lld: %s from cache\n",
        //        num, getCached(cache, num));
        return;
    }

    if (num >= HUNDRED) {
        // append numbers 100 - 1 quadrillion
        writeUpper(cache, num, write_to);
    } else {
        // append numbers 0-99
        writeLower(cache, num, write_to);
    }

    if (first)
        addToCache(cache, num, write_to);
}
