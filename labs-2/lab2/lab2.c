#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


#define SUCCESS 0
#define FAILURE 1

typedef struct st_argPrintText {
    char *text;
    int count;
} argPrintText;

void *printText(void *inputArg) {
    argPrintText *arg = (argPrintText *)inputArg;
    for (int i = 0; i < arg->count; ++i) {
        printf("%s", arg->text);
    }
    char * text = "text";
    pthread_exit(text);
}

void posixError(pthread_t threadID, int code, char *msg) {
    fprintf(stderr, "[ThreadID: %lu] %s: %s\n", threadID, msg, strerror(code));
    exit(EXIT_FAILURE);
}

int main() {
    pthread_t threadID;
    static argPrintText arg1 = {"Main thread\n", 10};
    static argPrintText arg2 = {"Another thread\n", 10};

    int retCode = pthread_create(&threadID, NULL, printText, (void *)&arg2);
    if (retCode != SUCCESS) {
        posixError(pthread_self(), retCode, "Error while creating new thread");
    }

    char *result;
    retCode = pthread_join(threadID, NULL);
    if (retCode != SUCCESS) {
        posixError(pthread_self(), retCode, "Error while joining to thread");
    }

    printText((void *)&arg1);
    return EXIT_SUCCESS;
}