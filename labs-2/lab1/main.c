#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *printText(void *data) {
    for (int i = 0; i < 10; ++i) {
        printf("%d\n", i);
    }
}

int main() {
    pthread_t threadID;
    int err = pthread_create(&threadID, NULL, printText, NULL);
    if (err != 0) {
        fprintf(stderr, "Error while creating thread! Code %d", err);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 10; ++i) {
        printf("%d\n", 100 - i);
    }
    return EXIT_SUCCESS;
}