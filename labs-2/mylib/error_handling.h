#ifndef NSU_OS_LABS_ERROR_HANDLING_H
#define NSU_OS_LABS_ERROR_HANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define SUCCESS 0
#define FAILURE 1
#define ERR_MSG_BUFFER_LEN 32
#define SEE_ERRNO_CODE -1

// Simple functions
void errorExit(char *errorMsg);

// Error handling in threads realization
typedef struct st_ThreadErrorState ThreadErrorState;
struct st_ThreadErrorState {
    pthread_t thID;
    int errCode;
};

ThreadErrorState createThreadErrState(pthread_t thID, int errCode);
int isThreadErrStateSuccess(ThreadErrorState state);
void threadErrorExit(ThreadErrorState state);
void printPosixThreadError(pthread_t threadID, int code);

#endif //NSU_OS_LABS_ERROR_HANDLING_H
