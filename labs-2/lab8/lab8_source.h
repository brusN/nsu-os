#ifndef NSU_OS_LABS_LAB8_SOURCE_H
#define NSU_OS_LABS_LAB8_SOURCE_H

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <limits.h>
#include "../mylib/mylib.h"
#include "../mylib/error_handling.h"

#define SUCCESS 0
#define FAILURE 1
#define CUR_INPUT_ARGS_NUMBER 3

typedef struct st_inputArgs inputArgs;
struct st_inputArgs {
    int numThreads;
    int numIterations;
};

typedef struct st_threadFuncArg threadFuncArg;
struct st_threadFuncArg {
    int iterNumber;
    int shift;
    double result;
};


// For function parseInputArgs(3)
enum parseInputArgs_states {
    parseInputArgs_SUCCESS = 0,
    parseInputArgs_INVALID_ARGS_COUNT,
    parseInputArgs_NOT_NUMBER,
    parseInputArgs_ERANGE,
    parseInputArgs_INVALID_NUM_THREADS,
    parseInputArgs_INVALID_NUM_ITERATIONS
};


int parseInputArgs(int argc, char **argv, inputArgs *inputArgsValues);
void invalidInputArgsExit(int retCode);
ThreadErrorState calcPi(int numThreads, int numIterations, double *result);
void distributeIterationsNumber(int numThreads, int numIterations, int *distribute);
threadFuncArg * createThreadsFunctionArgs(int numThreads, int numIterations, const int *distribute, threadFuncArg *args);
void * threadTask(void *arg);
ThreadErrorState createThreadsForTask(int numThreads, void *(*task)(void *), threadFuncArg *args, pthread_t *threadID);
ThreadErrorState collectResults(pthread_t *threadID, int numThreads, threadFuncArg *args, double *result);

#endif //NSU_OS_LABS_LAB8_SOURCE_H
