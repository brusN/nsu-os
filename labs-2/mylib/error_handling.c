#include "error_handling.h"

// Prints message and do exit(EXIT_FAILURE)
void errorExit(char *errorMsg) {
    fprintf(stderr, "[Error] %s\n", errorMsg);
    exit(EXIT_FAILURE);
}

ThreadErrorState createThreadErrState(pthread_t thID, int errCode) {
    ThreadErrorState state;
    state.thID = thID;
    state.errCode = errCode;
    return state;
}

int isThreadErrStateSuccess(ThreadErrorState state) {
    return state.errCode == 0;
}

void threadErrorExit(ThreadErrorState state) {
    int code;
    if (state.errCode == SEE_ERRNO_CODE) {
        code = errno;
    } else {
        code = state.errCode;
    }
    printPosixThreadError(state.thID, code);
    exit(EXIT_FAILURE);
}

void printPosixThreadError(pthread_t threadID, int code) {
    int bufferSize = ERR_MSG_BUFFER_LEN;
    char *errMsgBuffer = (char *)malloc(bufferSize);
    while (strerror_r(code, errMsgBuffer, bufferSize) != 0 && errno == ERANGE) {
        bufferSize *= 2;
        errMsgBuffer = realloc(errMsgBuffer, bufferSize);
    }
    fprintf(stderr, "[ThreadID: %lu] %s\n", threadID, strerror(code));
    free(errMsgBuffer);
}