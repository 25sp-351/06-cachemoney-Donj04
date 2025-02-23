#ifndef CONVERTER_H
#define CONVERTER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cache.h"

#define CACHE_SIZE 1500
#define STR_LENGTH 256

extern const long long QUADRILLION;
extern const long long TRILLION;
extern const long long BILLION;
extern const long long MILLION;
extern const long long THOUSAND;
extern const long long HUNDRED;

extern const char* ONES[];
extern const char* TEENS[];
extern const char* TENS_MULT[];
extern const char* POWERS_STR[];
extern const long long POWERS_NUM[];
extern const size_t POWERS_COUNT;

void appendStr(char* dest, const char* src);

// Sets all chars of str to '\0'
void clearStr(char* str);

// Print a given number as dollars and cents
// "{input} -> {dollars} dollars and {cents} cents"
// If input is negative:
// "{input} -> minus {dollars} dollars and {cents} cents"
int printAsMoney(Cache cache, const char* input);

// Appends uppper numbers to write_to ("hundred", "million", etc)
void writeUpper(Cache cache, long long num, char* write_to);

// Appends lower numbers to write_to ("three", "fifty one", etc)
void writeLower(Cache cache, long long num, char* write_to);

// Writes the string representation of num into write_to
void convertNumToText(Cache cache, long long num, char* write_to, bool first);

#endif
