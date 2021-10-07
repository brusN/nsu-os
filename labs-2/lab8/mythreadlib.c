#include "mythreadlib.h"


void printError(char *msg) {
    fprintf(stderr, "[Error] %s\n", msg);
    exit(EXIT_FAILURE);
}

void printPosixThreadError(pthread_t threadID, int code, char *msg) {
    int bufferSize = ERR_MSG_BUFFER_LEN;
    char *errMsgBuffer = (char *)malloc(bufferSize);
    while (strerror_r(code, , size) == -1 && errno == ERANGE) {
        bufferSize *= 2;
        errMsgBuffer = realloc(errMsgBuffer, bufferSize);
        if (errMsgBuffer == NULL) {
            printError("Error while allocating memory for error message buffer!");
        }
    }
    fprintf(stderr, "[ThreadID: %lu] %s: %s\n", threadID, msg, strerror(code));
    free(errMsgBuffer);
    exit(EXIT_FAILURE);
}