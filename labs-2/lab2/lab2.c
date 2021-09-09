#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SUCCESS 0
#define FAILURE 1

typedef struct st_argPrintText {
    char *text;
    int count;
} argPrintText;

void *printText(void *inputArg) {
    argPrintText *arg = inputArg;
    for (int i = 0; i < arg->count; ++i) {
        printf("%s\n", arg->text);
    }
}

int main() {
    pthread_t threadID;
    argPrintText arg1 = { "Main thread", 10};
    argPrintText arg2 = { "Another thread", 10};

    int err = pthread_create(&threadID, NULL, printText, &arg2);
    if (err != SUCCESS) {
        fprintf(stderr, "Error while creating thread! Code %d", err);
        exit(EXIT_FAILURE);
    }

    pthread_join(threadID, NULL);
    printText(&arg1);
}