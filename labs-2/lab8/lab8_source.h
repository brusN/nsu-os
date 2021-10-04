#ifndef NSU_OS_LABS_LAB8_SOURCE_H
#define NSU_OS_LABS_LAB8_SOURCE_H

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "mythreadlib.h"

#define CUR_INPUT_ARGS_NUMBER 2
#define NUM_ITERATIONS 200000000

typedef struct st_threadFuncArg threadFuncArg;
struct st_threadFuncArg {
    int iterNumber;
    int shift;
    double result;
};

void parseInputArguments(int argc, char **argv, int *threadsNumber);
int *distributeIterationsNumber(int iterationsNumber, int threadsNumber);
void * threadTask(void *arg);
threadFuncArg *createThreadsFunctionArgs(int numThreads, int numIterations);
pthread_t *createThreadsForTask(int numThreads, void *(*task)(void *), threadFuncArg *args);
double collectResults(pthread_t *threadID, int numThreads, threadFuncArg *args);

#endif //NSU_OS_LABS_LAB8_SOURCE_H
