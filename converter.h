#ifndef CONVERTER_H
#define CONVERTER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

// Writes the string representation of num into write_to
void writeNum(long long num, char* write_to, bool first);

// Appends upper numbers to write_to ("hundred", "million", etc)
void writeUpper(long long num, char* write_to);

// Appends lower numbers to write_to ("three", "fifty one", etc)
void writeLower(long long num, char* write_to);

// Convert money to string representation
// "{num} -> {dollars} dollars and {cents} cents"
// If input is negative:
// "{num} -> minus {dollars} dollars and {cents} cents"
char* moneyToStr(long long num);

#endif
