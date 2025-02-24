#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <stdbool.h>
#include <stdlib.h>

#define INPUT_OK 0
#define INPUT_INVALID 1
#define INPUT_OUT_OF_RANGE 2

#define CONVERT_SUCCESS 0
#define CONVERT_ERROR 3

#define USER_EXIT 0
#define USER_CONTINUE 4

#define BAD_Q_ARG 5
#define BAD_CACHE_ARG 6

#define BAD_ARG_COUNT 7
#define HAS_Q_ARG 8
#define NO_Q_ARG 9

extern const int MIN_ARGS;
extern const int MAX_ARGS;
extern const int Q_ARG;
extern const int CACHE_ARG;

extern const char* QUIET_COMMAND;
extern const char* NULL_CACHE;

extern const size_t BUFFER_SIZE;

extern const char* INPUT_LIMIT;
extern const size_t INPUT_MAX_DIGITS;

int getInput(char* buffer, bool hide_prompt);

int validateQuietCommand(int argc, char* argv[]);

// Return true if there is a potential argument for a cache and it is not "null"
bool canLoadCache(int argc, char* argv[]);

int validateArguments(int argc, char* argv[]);

// Returns true if input is a valid int and fits in a long long
int validateInput(const char* input);

// Return positive if input is greater than limit, negative if less, 0 if equal
int compareInput(const char* input, size_t length, size_t offset);

void clearInputBuffer();

void printError(int errcode, const char* input, bool descriptive);

#endif
