#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "error_handling.h"
#include "lab10_source.h"

int main(int argc, char **argv) {
    pthread_t philosophers[PHIL_COUNT];
    pthread_mutex_t forks[PHIL_COUNT];
    PhilTaskArg taskArgs[PHIL_COUNT];

    // Init mutex
    int returnCode = 0;
    for (int i = 0; i < PHIL_COUNT; ++i) {
        returnCode = pthread_mutex_init(&forks[i], NULL);
        if (returnCode != SUCCESS) {
            printPosixThreadError(pthread_self(), returnCode);
            exit(EXIT_FAILURE);
        }
    }

    // Creating args for philosopher tasks
    Food food;
    initFood(&food);
    createPhilTasks(taskArgs, PHIL_COUNT, &food, forks);

    // Creating threads for philosophers
    for (int i = 0; i < PHIL_COUNT; ++i) {
        returnCode = pthread_create(&philosophers[i], NULL, philTask, (void *)(&taskArgs[i]));
        if (returnCode != SUCCESS) {
            printPosixThreadError(pthread_self(), returnCode);
            exit(EXIT_FAILURE);
        }
    }

    // Joining threads
    for (int i = 0; i < PHIL_COUNT; ++i) {
        returnCode = pthread_join(philosophers[i], NULL);
        if (returnCode != SUCCESS) {
            printPosixThreadError(pthread_self(), returnCode);
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}

