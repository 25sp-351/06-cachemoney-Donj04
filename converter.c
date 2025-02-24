#include "converter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void writeNum(long long num, char* write_to, bool first) {
    if (num == 0 && !first)
        return;

    if (first)
        clearStr(write_to);

    if (num >= HUNDRED) {
        // append numbers 100 - 1 quadrillion
        writeUpper(num, write_to);
    } else {
        // append numbers 0-99
        writeLower(num, write_to);
    }
}

void writeUpper(long long num, char* write_to) {
    // find biggest power that num is greater than or equal to
    // (hundred, thousand, million, etc)
    for (size_t ix = 0; ix < POWERS_COUNT; ix++) {
        if (num >= POWERS_NUM[ix]) {
            char upper_str[STR_LENGTH];
            clearStr(upper_str);

            writeNum(num / POWERS_NUM[ix], upper_str, false);
            appendStr(upper_str, POWERS_STR[ix]);
            writeNum(num % POWERS_NUM[ix], upper_str, false);

            appendStr(write_to, upper_str);
            return;
        }
    }
}

void writeLower(long long num, char* write_to) {
    // only write numbers 0-99
    if (num >= HUNDRED)
        return;

    char lower_str[STR_LENGTH];
    clearStr(lower_str);

    if (num >= 20) {  // append tens place of numbers 20-99
        appendStr(lower_str, TENS_MULT[num / 10]);
        if (num % 10 > 0)  // append remaining ones
            appendStr(lower_str, ONES[num % 10]);

    } else if (num >= 10) {  // append numbers 10-19
        appendStr(lower_str, TEENS[num - 10]);

    } else {  // append numbers 0-9
        appendStr(lower_str, ONES[num]);
    }

    appendStr(write_to, lower_str);
}

char* moneyToStr(long long num) {
    long long cents;

    char* output = malloc(STR_LENGTH);

    snprintf(output, STR_LENGTH, "%lld ->", num);

    if (num < 0) {
        num = -num;
        appendStr(output, " minus");
    }
    cents = num % 100;
    num /= 100;

    char dollars_str[STR_LENGTH];
    char cents_str[STR_LENGTH];
    char* dollar_noun = (num == 1) ? " dollar and" : " dollars and";
    char* cent_noun   = (cents == 1) ? " cent" : " cents";

    writeNum(num, dollars_str, true);
    writeNum(cents, cents_str, true);

    // This could be done in one line with snprintf, but it caused compiler
    // warnings about the string being truncated that I wasn't sure how to fix
    appendStr(output, dollars_str);
    appendStr(output, dollar_noun);
    appendStr(output, cents_str);
    appendStr(output, cent_noun);

    return output;
}
