#include "lab24_source.h"

void signalHandler(int signum) {
    int countWidgets = getSemaphoreValue(&semWidgets);
    printf("Received SIGINT signal\n"
                "Stopping factory...\n"
                "Number of produced widgets: %d\n", countWidgets);

    exit(EXIT_SUCCESS);
}

int getSemaphoreValue(sem_t *sem) {
    int countWidgets;
    int returnCode = sem_getvalue(sem, &countWidgets);
    if (returnCode != SUCCESS) {
        printPosixThreadError(pthread_self(), returnCode);
        exit(EXIT_FAILURE);
    }
    return countWidgets;
}

