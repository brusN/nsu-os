#include "lab24_source.h"

int main() {
    // Setting signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // Starting fabric
    initSemaphores();
    int requiredCountWidgets = MAX_PRODUCES_WIDGET_COUNT;
    validatePosixThreadFuncResult(pthread_create(&widgetProducerThID, NULL, widgetProducerTask, (void *)&requiredCountWidgets), "Creating widget producer thread");
    validatePosixThreadFuncResult(pthread_join(widgetProducerThID, NULL), "Joining widget producer thread");
    printf("Production has finished\n");
    printProductionResult();
    destroySemaphores();
    return EXIT_SUCCESS;
}   
