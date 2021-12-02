#include "error_handling.h"

void printPosixThreadError(pthread_t threadID, int code, const char *comment) {
    char errMsgBuffer[ERR_MSG_BUFFER_LEN];
    if (strerror_r(code, errMsgBuffer, ERR_MSG_BUFFER_LEN) != SUCCESS) {
        if (errno == EINVAL) {
            fprintf(stderr, "Unknown error code!\n");
        }
        fprintf(stderr, "Insufficient storage to contain the generated message string!\n");
    } else {
        fprintf(stderr, "[ThreadID: %lu] %s >> %s\n", threadID, comment, errMsgBuffer);
    }
}