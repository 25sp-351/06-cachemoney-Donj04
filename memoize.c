#include <stdio.h>

#include "converter.h"
#include "inputmanager.h"

int main(int argc, char* argv[]) {
    Cache cache = newCache();
    char input[BUFFER_SIZE];

    while (true) {
        int user_state = getInput(argc, argv, input);

        if (user_state == USER_EXIT) {
            // Makes sure the command line is on the next line after previous
            // outputs
            printf("\n");
            break;

        } else if (user_state == USER_CONTINUE) {
            int input_state = inputValid(input);

            if (input_state != INPUT_OK) {
                printError(input_state, input, argc, argv);
            } else {
                input_state = printAsMoney(cache, input);
                if (input_state != CONVERT_SUCCESS)
                    printError(input_state, input, argc, argv);
            }

        } else if (user_state == USER_BAD_ARG) {
            printError(user_state, input, argc, argv);
            break;

        } else {
            printError(user_state, input, argc, argv);
        }
    }

    freeCache(cache);
    return 0;
}
