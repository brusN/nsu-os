#include "lab8_source.h"

int parseInputArgs(int argc, char **argv, inputArgs *inputArgsValues) {
    if (argc != CUR_INPUT_ARGS_NUMBER) {
        return parseInputArgs_INVALID_ARGS_COUNT;
    }

    // Num threads parsing ~ argv[1]
    long parsedNumThreads;
    int retCode = str2long(&parsedNumThreads, argv[1]);
    if (retCode == str2num_NOT_NUMBER) {
        return parseInputArgs_NOT_NUMBER;
    }
    if (retCode == str2num_ERANGE || parsedNumThreads <= 0 || parsedNumThreads > MAX_THREADS_COUNT) {
        return parseInputArgs_INVALID_NUM_THREADS;
    }
    inputArgsValues->numThreads = (int)parsedNumThreads;

    // Num iterations parsing ~ argv[2]
    long parsedNumIterations;
    retCode = str2long(&parsedNumIterations, argv[2]);
    if (retCode == str2num_NOT_NUMBER) {
        return parseInputArgs_NOT_NUMBER;
    }
    if (retCode == str2num_ERANGE || parsedNumIterations <= 0 || parsedNumIterations > MAX_ITERATIONS_COUNT) {
        return parseInputArgs_INVALID_NUM_ITERATIONS;
    }
    inputArgsValues->numIterations = (int)parsedNumIterations;

    return parseInputArgs_SUCCESS;
}

// Prints error message by using error return code and do exit(EXIT_FAILURE)
void invalidInputArgsExit(int retCode) {
    switch (retCode) {
        case parseInputArgs_INVALID_ARGS_COUNT:
            fprintf(stderr, "[Error] Invalid input args count! Excepted two arguments: number of threads, numbers of iteration\n");
            break;
        case parseInputArgs_NOT_NUMBER:
            fprintf(stderr, "[Error] Input argument isn't a number\n");
            break;
        case parseInputArgs_ERANGE:
            fprintf(stderr, "[Error] Input argument value is out of integer range\n");
            break;
        case parseInputArgs_INVALID_NUM_THREADS:
            fprintf(stderr, "[Error] Invalid num threads! Must be > 0. Threads count limit: %d\n", MAX_THREADS_COUNT);
            break;
        case parseInputArgs_INVALID_NUM_ITERATIONS:
            fprintf(stderr, "[Error] Invalid num iterations! Must be > 0. Iterations count limit: %d\n", MAX_ITERATIONS_COUNT);
            break;
        default:
            fprintf(stderr, "[Error] Unknown error return code\n");
    }
    exit(EXIT_FAILURE);
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
        if (i % 2 == 0) {
            threadArg->result += 1.0 / (i * 2.0 + 1.0);
        } else {
            threadArg->result -= 1.0 / (i * 2.0 + 1.0);
        }
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
    int distribute[numThreads];
    distributeIterationsNumber(numThreads, numIterations, distribute);

    // Creating arguments for thread functions
    threadFuncArg threadArgs[numThreads];
    createThreadsFunctionArgs(numThreads, numIterations, distribute, threadArgs);

    // Creating threads for calculating
    pthread_t threadID[numThreads];
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



