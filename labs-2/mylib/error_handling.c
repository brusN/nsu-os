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
    return state.errCode == SUCCESS;
}

void threadErrorExit(ThreadErrorState state) {
    printPosixThreadError(state.thID, state.errCode);
    exit(EXIT_FAILURE);
}

void printPosixThreadError(pthread_t threadID, int code) {
    char errMsgBuffer[ERR_MSG_BUFFER_LEN];
    if (strerror_r(code, errMsgBuffer, ERR_MSG_BUFFER_LEN) != SUCCESS) {
        if (errno == EINVAL) {
            fprintf(stderr, "Unknown error code!\n");
        }
        fprintf(stderr, "Insufficient storage to contain the generated message string!\n");
    } else {
        fprintf(stderr, "[ThreadID: %lu] %s\n", threadID, errMsgBuffer);
    }
}