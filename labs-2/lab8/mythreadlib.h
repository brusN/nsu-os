#ifndef NSU_OS_LABS_MYTHREADLIB_H
#define NSU_OS_LABS_MYTHREADLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define SUCCESS 0
#define FAILURE 1

void printError(char *msg);
void printPosixThreadError(pthread_t threadID, int code, char *msg);

#endif //NSU_OS_LABS_MYTHREADLIB_H
