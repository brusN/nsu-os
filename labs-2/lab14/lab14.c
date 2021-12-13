#include <stdio.h>
#include "lab14_source.h"
#include <pthread.h>

int main(int argc, char **argv) {
    int countRows;
    int retCode = parseInputArguments(argc, argv, &countRows);
    if (retCode != SUCCESS) {
        printParseInputArgsError(retCode);
        exit(EXIT_FAILURE);
    }
    retCode = initSemaphores();
    if (retCode != SUCCESS) {
        // Error description has printed by function initSemaphores()
        exit(EXIT_FAILURE);
    }
    pthread_cleanup_push(cleanupOnParentThreadExit, NULL)
            parentThreadID = pthread_self();
            retCode = pthread_create(&childThreadID, NULL, childThreadTask, (void *) &countRows);
            if (retCode != SUCCESS) {
                printPosixThreadError(pthread_self(), retCode, "Creating child thread");
                pthread_exit(PTHREAD_CANCELED);
            }
            for (int rowNumber = 0; rowNumber < countRows; ++rowNumber) {
                retCode = sem_wait(&childThSem);
                if (retCode != SUCCESS) {
                    printPosixThreadError(pthread_self(), retCode, "Wait child thread semaphore");
                    retCode = pthread_cancel(childThreadID);
                    if (retCode != SUCCESS) {
                        printPosixThreadError(pthread_self(), retCode, "Cancelling child thread");
                        exit(EXIT_FAILURE);
                    }
                    pthread_exit(PTHREAD_CANCELED);
                }
                printf("[P] Text #%d\n", rowNumber);
                retCode = sem_post(&parentThSem);
                if (retCode != SUCCESS) {
                    printPosixThreadError(pthread_self(), retCode, "Post parent thread semaphore");
                    retCode = pthread_cancel(childThreadID);
                    if (retCode != SUCCESS) {
                        printPosixThreadError(pthread_self(), retCode, "Cancelling child thread");
                        exit(EXIT_FAILURE);
                    }
                    pthread_exit(PTHREAD_CANCELED);
                }
            }
            retCode = pthread_join(childThreadID, NULL);
            if (retCode != SUCCESS) {
                printPosixThreadError(pthread_self(), retCode, "Joining child thread");
                pthread_exit(PTHREAD_CANCELED);
            }
    pthread_cleanup_pop(EXECUTE_CLEANUP_FUNC);
    return EXIT_SUCCESS;
}
