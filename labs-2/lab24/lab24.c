#include "lab24_source.h"

int main(int argc, char **argv) {
    int requiredCountWidgets;
    int returnCode = parseInputArguments(argc, argv, &requiredCountWidgets);
    if (returnCode != parseInputArgs_SUCCESS) {
        printParseInputArgsError(returnCode);
        exit(EXIT_FAILURE);
    }

    int retCode = initSemaphores();
    if (retCode != SUCCESS) {
        // Error describe has printed by initSemaphores() function
        fprintf(stderr, "[Error] Got error while initializing semaphores\n");
        if (destroySemaphores() != SUCCESS) {
            // Error describe has printed by destroySemaphores() function
            fprintf(stderr, "[Error] Got error while destroying semaphores\n");
        }
        exit(EXIT_FAILURE);
    }

    // It's contains info about threads which is executing
    retCode = initThreadsInfo(&threadsInfo);
    if (retCode != SUCCESS) {
        fprintf(stderr, "[Error] Error while initializing threads info struct\n");
        exit(EXIT_FAILURE);
    }
    retCode = produceWidgets(requiredCountWidgets);
    if (retCode != SUCCESS) {
        fprintf(stderr, "[Error] Error while producing widgets\n");
        exit(EXIT_FAILURE);
    }
    printf("Widgets production has finished\n");
    retCode = destroyThreadsInfo(&threadsInfo);
    if (retCode != SUCCESS) {
        fprintf(stderr, "[Error] Error while destroying threads info struct\n");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
