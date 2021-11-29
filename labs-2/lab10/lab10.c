#include "lab10_source.h"
#include "error_handling.h"

int main() {
    // Contains info about initialized resources, which signal to clean resources function
    // which resources should be free after success or fail execution
    ResourcesInfo initializedResourcesInfo;
    initResourcesInfo(&initializedResourcesInfo);

    // Initializing fork mutexes
    int retCode = initMutexes(forks, PHIL_COUNT);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Initializing forks mutexes");
        exit(EXIT_FAILURE);
    }
    initializedResourcesInfo.isForksMutexesInitialized = RESOURCE_INITIALIZED;

    // Initializing food and eat lock mutex
    retCode = initFood(&food, FOOD_INIT_COUNT);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Initializing food");
        cleanResources(&initializedResourcesInfo);
        exit(EXIT_FAILURE);
    }
    initializedResourcesInfo.isFoodMutexInitialized = RESOURCE_INITIALIZED;

    // Creating threads for philosophers
    PhilTaskArg taskArgs[PHIL_COUNT];
    initPhilTaskArgs(taskArgs, PHIL_COUNT, &food);
    retCode = createPhilsThreads(philsThID, PHIL_COUNT, taskArgs);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Creating threads for philosophers");
        cleanResources(&initializedResourcesInfo);
        exit(EXIT_FAILURE);
    }

    // Joining philosophers threads
    retCode = joinPhilsThreads(philsThID, PHIL_COUNT);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Joining philosophers threads");
        cleanResources(&initializedResourcesInfo);
        exit(EXIT_FAILURE);
    }

    cleanResources(&initializedResourcesInfo);
    return EXIT_SUCCESS;
}