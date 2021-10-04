#include "mythreadlib.h"

void printError(char *msg) {
    fprintf(stderr, "[Error] %s\n", msg);
    exit(EXIT_FAILURE);
}

void printPosixThreadError(pthread_t threadID, int code, char *msg) {
    fprintf(stderr, "[ThreadID: %lu] %s: %s\n", threadID, msg, strerror(code));
    exit(EXIT_FAILURE);
}