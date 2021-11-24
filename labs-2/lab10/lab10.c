#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "error_handling.h"
#include "lab10_source.h"

int main(int argc, char **argv) {
    validatePosixThreadFuncResult(initMutexes(), "Initiating mutexes");

    // Creating args for philosopher tasks
    Food food;
    validatePosixThreadFuncResult(initFood(&food), "Initiating food");
    PhilTaskArg taskArgs[PHIL_COUNT];
    createPhilTasks(taskArgs, PHIL_COUNT, &food);

    // Creating threads for philosophers
    pthread_t philosophers[PHIL_COUNT];
    for (int i = 0; i < PHIL_COUNT; ++i) {
        validatePosixThreadFuncResult(pthread_create(&philosophers[i], NULL, philTask, (void *)(&taskArgs[i])), "Creating threads for philosophers");
    }
    for (int i = 0; i < PHIL_COUNT; ++i) {
        validatePosixThreadFuncResult(pthread_join(philosophers[i], NULL), "Joining threads");
    }

    // Free resources
    validatePosixThreadFuncResult(destroyMutexes(), "Destroying mutexes");
    return EXIT_SUCCESS;
}

