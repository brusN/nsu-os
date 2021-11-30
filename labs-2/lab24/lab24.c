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
    pthread_create(&widgetProducerThID, NULL, widgetProducerTask, (void *) &requiredCountWidgets);
    pthread_join(widgetProducerThID, NULL);
    printf("Production has finished\n");

    printProductionResult();
    destroySemaphores();
    return EXIT_SUCCESS;
}   
