#include "lab14_source.h"

int parseInputArguments(int argc, char **argv, int *countRows) {
    if (argc != REQUIRED_COUNT_INPUT_ARGS) {
        return parseInputArgs_ILLEGAL_COUNT_INPUT_ARGS;
    }
    long parsedRequiredCountRows;
    int returnCode = str2long(&parsedRequiredCountRows, argv[1]);
    if (returnCode == str2num_NOT_NUMBER || returnCode == str2num_EMPTY_STR) {
        return parseInputArgs_NOT_A_NUMBER;
    }
    if (returnCode == str2num_ERANGE || parsedRequiredCountRows > MAX_COUNT_ROWS || parsedRequiredCountRows <= 0) {
        return parseInputArgs_OUT_OF_RANGE;
    }
    *countRows = (int) parsedRequiredCountRows;
    return parseInputArgs_SUCCESS;
}

void printParseInputArgsError(int errCode) {
    switch (errCode) {
        case parseInputArgs_NOT_A_NUMBER:
            fprintf(stderr, "[Error] Input argument not a number\n");
            break;
        case parseInputArgs_OUT_OF_RANGE:
            fprintf(stderr,
                    "[Error] Input argument out of range! Number must be positive and less than max count constraint %d\n",
                    MAX_COUNT_ROWS);
            break;
        case parseInputArgs_ILLEGAL_COUNT_INPUT_ARGS:
            fprintf(stderr,
                    "[Error] Illegal count arguments! Must entered positive number and less than max count constraint %d\n",
                    MAX_COUNT_ROWS);
            break;
        default:
            fprintf(stderr, "[Error] Unknown error code\n");
            break;
    }
}

int initSemaphores() {
    int retCode = sem_init(&parentThSem, NO_PROCESS_SHARED, 0);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Initializing parent thread semaphore");
        return FAILURE;
    }
    retCode = sem_init(&childThSem, NO_PROCESS_SHARED, 1);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Initializing child thread semaphore");
        int destroyParentSemRetCode = sem_destroy(&parentThSem);
        if (destroyParentSemRetCode != SUCCESS) {
            printPosixThreadError(pthread_self(), errno, "Destroying parent thread semaphore on error");
        }
        return FAILURE;
    }
    return SUCCESS;
}

int destroySemaphores() {
    int retCode = sem_destroy(&parentThSem);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Destroying parent thread semaphore");
        return FAILURE;
    }
    retCode = sem_destroy(&childThSem);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Destroying child thread semaphore");
        return FAILURE;
    }
    return SUCCESS;
}

void cleanupOnParentThreadExit(void *arg) {
    printf("Parent cleanup!\n");
    int retCode = pthread_cancel(childThreadID);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Cancelling child thread");
        exit(EXIT_FAILURE);
    }
    retCode = sem_destroy(&childThSem);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Destroying child thread semaphore");
        exit(EXIT_FAILURE);
    }
    retCode = sem_destroy(&parentThSem);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Destroying parent thread semaphore");
        exit(EXIT_FAILURE);
    }
    printf("Parent cleanup finish!\n");
}

void *childThreadTask(void *arg) {
    int countRows = *(int *) arg;
    int retCode;
    for (int rowNumber = 0; rowNumber < countRows; ++rowNumber) {
        retCode = sem_wait(&parentThSem);
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Wait parent thread semaphore");
            int cancelParentThreadRetCode = pthread_cancel(parentThreadID);
            if (cancelParentThreadRetCode != SUCCESS) {
                printPosixThreadError(pthread_self(), cancelParentThreadRetCode,
                                      "Cancelling parent thread on error\n");
                exit(EXIT_FAILURE);
            }
            pthread_exit(PTHREAD_CANCELED);
        }
        printf("[C] Text #%d\n", rowNumber);
        retCode = sem_post(&childThSem);
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Post child thread semaphore");
            int cancelParentThreadRetCode = pthread_cancel(parentThreadID);
            if (cancelParentThreadRetCode != SUCCESS) {
                printPosixThreadError(pthread_self(), cancelParentThreadRetCode,
                                      "Cancelling parent thread on error\n");
                exit(EXIT_FAILURE);
            }
            pthread_exit(PTHREAD_CANCELED);
        }
    }
    pthread_exit(NULL);
}