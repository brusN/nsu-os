#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "source.h"

#define NO_OPTIONS -1

int main(int argc, char *argv[]) {
    // If haven't any options
    if (argc < 2)
        printUsage();

    int optionID = NO_OPTIONS;
    char *optionsList = "ispuU:cC:dvV:";
    optionID = getopt(argc, argv, optionsList);
    while (optionID != NO_OPTIONS) {
        if (executeCommand(optionID) == STATUS_FAIL) {
            printf("Error while executing command!\n");
            exit(1);
        }
	printf("\n");
        optionID = getopt(argc, argv, optionsList);
    }
    return 0;
}
