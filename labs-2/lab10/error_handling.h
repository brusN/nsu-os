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

void printPosixThreadError(pthread_t threadID, int code, const char *comment);

#endif //NSU_OS_LABS_ERROR_HANDLING_H
