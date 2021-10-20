#include "lab8_source.h"

int parseInputArgs(int argc, char **argv, inputArgs *inputArgsValues) {
    if (argc != CUR_INPUT_ARGS_NUMBER) {
        return parseInputArgs_INVALID_ARGS_COUNT;
    }

    // Num threads parsing ~ argv[1]
    long parsedNumThreads;
    int retCode = str2long(&parsedNumThreads, argv[1], NULL, 10);
    if (retCode != str2num_SUCCESS) {
        if (retCode == str2num_ERANGE) {
            return parseInputArgs_ERANGE;
        } else {
            return parseInputArgs_NOT_NUMBER;
        }
    }
    if (parsedNumThreads < INT_MAX || parsedNumThreads > INT_MAX) {
        return parseInputArgs_ERANGE;
    }
    if (parsedNumThreads <= 0) {
        return parseInputArgs_INVALID_NUM_THREADS;
    }
    inputArgsValues->numThreads = (int)parsedNumThreads;

    // Num iterations parsing ~ argv[2]
    long parsedNumIterations;
    retCode = str2long(&parsedNumIterations, argv[2], NULL, 10);
    if (retCode != str2num_SUCCESS) {
        if (retCode == str2num_ERANGE) {
            return parseInputArgs_ERANGE;
        } else {
            return parseInputArgs_NOT_NUMBER;
        }
    }
    if (parsedNumIterations < INT_MIN || parsedNumIterations > INT_MAX) {
        return parseInputArgs_ERANGE;
    }
    if (parsedNumIterations <= 0) {
        return parseInputArgs_INVALID_NUM_ITERATIONS;
    }
    inputArgsValues->numIterations = (int)parsedNumIterations;

    return parseInputArgs_SUCCESS;
}

// Prints error message by using error return code and do exit(EXIT_FAILURE)
void invalidInputArgsExit(int retCode) {
    switch (retCode) {
        case parseInputArgs_INVALID_ARGS_COUNT:
            errorExit("Invalid input args count! Excepted two arguments: number of threads, numbers of iterations");
        case parseInputArgs_NOT_NUMBER:
            errorExit("Input argument isn't a number");
        case parseInputArgs_ERANGE:
            errorExit("Input argument value is out of integer range");
        case parseInputArgs_INVALID_NUM_THREADS:
            errorExit("Invalid num threads! Must be > 0");
        case parseInputArgs_INVALID_NUM_ITERATIONS:
            errorExit("Invalid num iterations! Must be > 0");
        default:
            errorExit("Unknown error return code");
    }
}

void distributeIterationsNumber(int numThreads, int numIterations, int *distribute) {
    int iterationsPerThread = numIterations / numThreads;
    for (int i = 0; i < numThreads; ++i) {
        distribute[i] = iterationsPerThread;
    }

    // If the number of iterations is not a multiple of the number of required threads
    int remainder = numIterations % numThreads;
    if (remainder != 0) {
        for (int i = 0; i < remainder; ++i) {
            distribute[i] += 1;
        }
    }
}

threadFuncArg * createThreadsFunctionArgs(int numThreads, int numIterations, const int *distribute, threadFuncArg *args) {
    args[0].iterNumber = distribute[0];
    args[0].shift = 0;
    args[0].result = 0;
    for (int i = 1; i < numThreads; ++i) {
        args[i].iterNumber = distribute[i];
        args[i].shift = args[i - 1].shift + distribute[i - 1];
        args[i].result = 0;
    }
    return args;
}

void * threadTask(void *arg) {
    threadFuncArg *threadArg = (threadFuncArg *)arg;
    for (int i = threadArg->shift; i < threadArg->shift + threadArg->iterNumber; ++i) {
        threadArg->result += 1.0 / (i * 4.0 + 1.0);
        threadArg->result -= 1.0 / (i * 4.0 + 3.0);
    }
    pthread_exit(&threadArg->result);
}

ThreadErrorState createThreadsForTask(int numThreads, void *(*task)(void *), threadFuncArg *args, pthread_t *threadID) {
    int retCode;
    for (int i = 0; i < numThreads; ++i) {
        retCode = pthread_create(&threadID[i], NULL, task, &args[i]);
        if (retCode != SUCCESS) {
            return createThreadErrState(pthread_self(), retCode);
        }
    }
    return createThreadErrState(pthread_self(), SUCCESS);
}

ThreadErrorState collectResults(pthread_t *threadID, int numThreads, threadFuncArg *args, double *result) {
    *result = 0;
    void *threadResult;
    int retCode;
    for (int i = 0; i < numThreads; ++i) {
        retCode = pthread_join(threadID[i], &threadResult);
        if (retCode != SUCCESS) {
            return createThreadErrState(pthread_self(), retCode);
        }
        *result += *(double *)threadResult;
    }
    *result *= 4.0;
    return createThreadErrState(pthread_self(), SUCCESS);
}

ThreadErrorState calcPi(int numThreads, int numIterations, double *result) {

    // Distributing parts to calculating between threads
    int *distribute = (int *)malloc(sizeof(int) * numThreads);
    if (distribute == NULL) {
        return createThreadErrState(pthread_self(), SEE_ERRNO_CODE);
    }
    distributeIterationsNumber(numThreads, numIterations, distribute);

    // Creating arguments for thread functions
    threadFuncArg *threadArgs = (threadFuncArg *)malloc(sizeof(threadFuncArg) * numThreads);
    if (threadArgs == NULL) {
        return createThreadErrState(pthread_self(), SEE_ERRNO_CODE);
    }
    threadArgs = createThreadsFunctionArgs(numThreads, numIterations, distribute, threadArgs);
    free(distribute);

    // Creating threads for calculating
    pthread_t *threadID = (pthread_t *)malloc(sizeof(pthread_t) * numThreads);
    if (threadID == NULL) {
        return createThreadErrState(pthread_self(), SEE_ERRNO_CODE);
    }
    ThreadErrorState state = createThreadsForTask(numThreads, threadTask, threadArgs, threadID);
    if (!isThreadErrStateSuccess(state)) {
        return state;
    }

    state = collectResults(threadID, numThreads, threadArgs, result);
    if (!isThreadErrStateSuccess(state)) {
        return state;
    }
    return createThreadErrState(pthread_self(), SUCCESS);
}



