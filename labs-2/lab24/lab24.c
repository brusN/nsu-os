#include "lab24_source.h"

int main(int argc, char **argv) {
    int requiredCountWidgets;
    int returnCode = parseInputArguments(argc, argv, &requiredCountWidgets);
    if (returnCode != parseInputArgs_SUCCESS) {
        printParseInputArgsError(returnCode);
        return EXIT_FAILURE;
    }

    // Setting signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // Starting fabric
    initSemaphores();
    validatePosixThreadFuncResult(
            pthread_create(&widgetProducerThID, NULL, widgetProducerTask, (void *) &requiredCountWidgets),
            "Creating widget producer thread");
    validatePosixThreadFuncResult(pthread_join(widgetProducerThID, NULL), "Joining widget producer thread");
    printf("Production has finished\n");
    printProductionResult();
    destroySemaphores();
    return EXIT_SUCCESS;
}   
