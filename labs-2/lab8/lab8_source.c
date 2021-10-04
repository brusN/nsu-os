#include "lab8_source.h"

void parseInputArguments(int argc, char **argv, int *threadsNumber) {
    if (argc != CUR_INPUT_ARGS_NUMBER) {
        printError("Wrong number of input arguments");
    }
    *threadsNumber = (int)strtol(argv[1], NULL, 10);
    if (*threadsNumber == 0) {
        printError("Error while converting input argument to number: string isn't integer number");
    } else if (errno == ERANGE) {
        printError("Error while converting input argument to number: the correct value is out of range");
    }
}

int *distributeIterationsNumber(int iterationsNumber, int threadsNumber) {
    int *distribute = (int *)malloc(sizeof(int) * threadsNumber);
    if (distribute == NULL) {
        printError("Error while allocating memory for distribute array");
    }

    int iterationsPerThread = iterationsNumber / threadsNumber;
    for (int i = 0; i < threadsNumber; ++i) {
        distribute[i] = iterationsPerThread;
    }

    // If the number of iterations is not a multiple of the number of required threads
    int remainder = iterationsNumber % threadsNumber;
    if (remainder != 0) {
        for (int i = 0; i < remainder; ++i) {
            distribute[i] += 1;
        }
    }

    return distribute;
}

threadFuncArg *createThreadsFunctionArgs(int numThreads, int numIterations) {
    threadFuncArg *threadArg = (threadFuncArg *)malloc(sizeof(threadFuncArg) * numThreads);
    if (threadArg == NULL) {
        printError("Error while allocating memory for thread arguments array");
    }

    int *distribute = distributeIterationsNumber(numIterations, numThreads);
    threadArg[0].iterNumber = distribute[0];
    threadArg[0].shift = 0;
    threadArg[0].result = 0;
    for (int i = 1; i < numThreads; ++i) {
        threadArg[i].iterNumber = distribute[i];
        threadArg[i].shift = threadArg[i - 1].shift + distribute[i - 1];
        threadArg[i].result = 0;
    }
    free(distribute);
    return threadArg;
}

void * threadTask(void *arg) {
    threadFuncArg *threadArg = (threadFuncArg *)arg;
    for (int i = threadArg->shift; i < threadArg->shift + threadArg->iterNumber; ++i) {
        threadArg->result += 1.0 / (i * 4.0 + 1.0);
        threadArg->result -= 1.0 / (i * 4.0 + 3.0);
    }
    pthread_exit(&threadArg->result);
}

pthread_t *createThreadsForTask(int numThreads, void *(*task)(void *), threadFuncArg *args) {
    pthread_t *threadID = (pthread_t *)malloc(sizeof(pthread_t) * numThreads);
    if (threadID == NULL) {
        printError("Error while allocating memory for threads id array");
    }

    int retCode;
    for (int i = 0; i < numThreads; ++i) {
        retCode = pthread_create(&threadID[i], NULL, task, &args[i]);
        if (retCode != SUCCESS) {
            free(threadID);
            free(args);
            printPosixThreadError(pthread_self(), retCode, "Error while creating thread");
        }
    }
    return threadID;
}

double collectResults(pthread_t *threadID, int numThreads, threadFuncArg *args) {
    double result = 0;
    void *threadResult;
    int retCode;
    for (int i = 0; i < numThreads; ++i) {
        retCode = pthread_join(threadID[i], &threadResult);
        if (retCode != SUCCESS) {
            free(threadID);
            free(args);
            printPosixThreadError(pthread_self(), retCode, "Error while joining thread");
        }
        result += *(double *)threadResult;
    }
    return result;
}

