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
#define USER_BAD_ARG 5

#define HAS_Q_ARG 6
#define NO_Q_ARG 7

extern const char* QUIET_COMMAND;

extern const size_t BUFFER_SIZE;

extern const char* LLONG_MAX_STR;
extern const size_t LLONG_MAX_DIGITS;

// Check if input number is within bounds of a long long int.
// Only use this if input has same decimal digits as the max/min
int inputInRange(const char* input, size_t length, size_t idx_offset);

// Returns true if input is a valid int and fits in a long long
int inputValid(const char* input);

void printError(int errcode, const char* input, int argc, char* argv[]);

int hasQuietCommand(int argc, char* argv[]);

int getInput(int argc, char* argv[], char* buffer);

#endif
