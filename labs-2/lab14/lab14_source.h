#ifndef NSU_OS_LABS_LAB14_SOURCE_H
#define NSU_OS_LABS_LAB14_SOURCE_H

#include <semaphore.h>
#include "error_handling.h"
#include "mylib.h"

#define REQUIRED_COUNT_INPUT_ARGS 2
#define MAX_COUNT_ROWS 1000
#define NO_PROCESS_SHARED 1
#define PROCESS_SHARED 0
#define EXECUTE_CLEANUP_FUNC 1
#define DONT_EXECUTE_CLEANUP_FUNC 0

pthread_t parentThreadID, childThreadID;
sem_t parentThSem, childThSem;

enum parseInputArgs_state {
    parseInputArgs_SUCCESS = 0,
    parseInputArgs_ILLEGAL_COUNT_INPUT_ARGS,
    parseInputArgs_OUT_OF_RANGE,
    parseInputArgs_NOT_A_NUMBER
};

// Handling input args
int parseInputArguments(int argc, char **argv, int *countRows);
void printParseInputArgsError(int errCode);

// Semaphores
int initSemaphores();
int destroySemaphores();

// Threads
void cleanupOnParentThreadExit(void *arg);
void *childThreadTask(void *arg);


#endif //NSU_OS_LABS_LAB14_SOURCE_H
