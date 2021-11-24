#include "error_handling.h"
#include "../mylib/error_handling.h"

// Handling the return value of pthread functions
void validatePosixThreadFuncResult(int returnCode, const char *comment) {
    if (returnCode != SUCCESS) {
        printPosixThreadError(pthread_self(), returnCode, comment);
        exit(EXIT_FAILURE);
    }
}

void printPosixThreadError(pthread_t threadID, int code, const char *comment) {
    char errMsgBuffer[ERR_MSG_BUFFER_LEN];
    if (strerror_r(code, errMsgBuffer, ERR_MSG_BUFFER_LEN) != SUCCESS) {
        if (errno == EINVAL) {
            fprintf(stderr, "Unknown error code!\n");
        }
        fprintf(stderr, "Insufficient storage to contain the generated message string!\n");
    } else {
        fprintf(stderr, "[ThreadID: %lu] Where >> %s | %s\n", threadID, comment, errMsgBuffer);
    }
}
