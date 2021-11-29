#include "lab10_source.h"

void initResourcesInfo(ResourcesInfo *resourcesInfo) {
    resourcesInfo->isFoodMutexInitialized = RESOURCE_NOT_INITIALIZED;
    resourcesInfo->isForksMutexesInitialized = RESOURCE_NOT_INITIALIZED;
}

int cleanResources(ResourcesInfo *resourcesInfo) {
    int wasCleanResourcesError = FALSE;
    int retCode;
    if (resourcesInfo->isForksMutexesInitialized) {
        for (int i = 0; i < PHIL_COUNT; ++i) {
            retCode = pthread_mutex_destroy(&forks[i]);
            if (retCode != SUCCESS) {
                printPosixThreadError(pthread_self(), retCode, "Destroying fork mutex in clean resources");
                wasCleanResourcesError = TRUE;
            }
        }
    }

    if (resourcesInfo->isFoodMutexInitialized) {
        retCode = pthread_mutex_destroy(&food.eatLock);
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Destroying food lock mutex in clean resources");
            wasCleanResourcesError = TRUE;
        }
    }

    if (wasCleanResourcesError == TRUE) {
        return FAIL;
    }
    return SUCCESS;
}

int initMutexesAttribute(pthread_mutexattr_t *attr, int mutexType, int mutexRobust) {
    int retCode = pthread_mutexattr_init(attr);
    if (retCode != SUCCESS) {
        return retCode;
    }

    retCode = pthread_mutexattr_setrobust(attr, mutexRobust);
    if (retCode != SUCCESS) {
        int destroyMutexRetCode = pthread_mutexattr_destroy(attr);
        if (destroyMutexRetCode != SUCCESS) {
            printPosixThreadError(pthread_self(), destroyMutexRetCode, "Destroying mutex on error");
        }
        return retCode;
    }

    retCode = pthread_mutexattr_settype(attr, mutexType);
    if (retCode != SUCCESS) {
        int destroyMutexRetCode = pthread_mutexattr_destroy(attr);
        if (destroyMutexRetCode != SUCCESS) {
            printPosixThreadError(pthread_self(), destroyMutexRetCode, "Destroying mutex on error");
        }
    }
    return retCode;
}

int initMutexes(pthread_mutex_t *mutexes, int count) {
    pthread_mutexattr_t attributes;
    int retCode = initMutexesAttribute(&attributes, PTHREAD_MUTEX_ERRORCHECK, PTHREAD_MUTEX_ROBUST);
    if (retCode != SUCCESS) {
        return retCode;
    }
    for (int i = 0; i < count; ++i) {
        retCode = pthread_mutex_init(&mutexes[i], &attributes);
        if (retCode != SUCCESS) {
            int cleanResourcesRetCode = destroyMutexes(forks, count);
            if (cleanResourcesRetCode != SUCCESS) {
                printPosixThreadError(pthread_self(), cleanResourcesRetCode, "Destroying mutexes on error");
            }
            return retCode;
        }
    }
    retCode = pthread_mutexattr_destroy(&attributes);
    if (retCode != SUCCESS) {
        int cleanResourcesRetCode = destroyMutexes(forks, count);
        if (cleanResourcesRetCode != SUCCESS) {
            printPosixThreadError(pthread_self(), cleanResourcesRetCode, "Destroying mutexes on error");
        }
        return retCode;
    }
    return SUCCESS;
}

int destroyMutexes(pthread_mutex_t *mutexes, int count) {
    int retCode;
    for (int i = 0; i < count; ++i) {
        retCode = pthread_mutex_destroy(&mutexes[i]);
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Destroying mutex");
            return retCode;
        }
    }
    return SUCCESS;
}

int initFood(Food *foodInfo, int initCount) {
    foodInfo->foodLeft = initCount;
    return pthread_mutex_init(&foodInfo->eatLock, NULL);
}

void initPhilTaskArgs(PhilTaskArg *taskArgs, int countPhils, Food *foodInfo) {
    for (int i = 0; i < countPhils; ++i) {
        taskArgs[i].philNumber = i;
        taskArgs[i].food = foodInfo;
        taskArgs[i].leftForkMutexID = i;
        taskArgs[i].rightForkMutexID = (i + 1 == countPhils ? 0 : i + 1);
    }
}

void *philTask(void *arg) {
    pthread_cleanup_push(cleanupPhilThreadsOnError, arg) // Sets cleaner for thread after pthread_exit
            PhilTaskArg *taskArg = (PhilTaskArg *) arg;
            printf("Philosopher %d sitting down to dinner.\n", taskArg->philNumber);
            int foodLeft;
            int retCode = takeFood(taskArg->food, &foodLeft);
            if (retCode != SUCCESS) {
                retCode = cancelPhilsThreads();
                if (retCode != SUCCESS) {
                    printPosixThreadError(pthread_self(), retCode, "Canceling philosophers threads");
                    exit(EXIT_FAILURE);
                }
            }

            while (foodLeft) {
                // Thinks
                printf("[Phil %d] Thinking...\n", taskArg->philNumber);
                usleep(THINK_TIME);

                // Returns to dinner
                printf("[Phil %d] Getting dish %d\n", taskArg->philNumber, foodLeft);
                retCode = getForks(taskArg->philNumber, taskArg->leftForkMutexID, taskArg->rightForkMutexID);
                if (retCode != SUCCESS) {
                    retCode = cancelPhilsThreads();
                    if (retCode != SUCCESS) {
                        printPosixThreadError(pthread_self(), retCode, "Canceling philosophers threads");
                        exit(EXIT_FAILURE);
                    }
                }
                printf("[Phil %d] Eating\n", taskArg->philNumber);
                usleep(DELAY * (FOOD_INIT_COUNT - foodLeft + 1));
                retCode = downForks(taskArg->philNumber, taskArg->leftForkMutexID, taskArg->rightForkMutexID);
                if (retCode != SUCCESS) {
                    retCode = cancelPhilsThreads();
                    if (retCode != SUCCESS) {
                        printPosixThreadError(pthread_self(), retCode, "Canceling philosophers threads");
                        exit(EXIT_FAILURE);
                    }
                }

                // Take a new portion of food, if there is
                retCode = takeFood(taskArg->food, &foodLeft);
                if (retCode != SUCCESS) {
                    retCode = cancelPhilsThreads();
                    if (retCode != SUCCESS) {
                        printPosixThreadError(pthread_self(), retCode, "Canceling philosophers threads");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            printf("[Phil %d] Done eating\n", taskArg->philNumber);
    pthread_cleanup_pop(DO_NOT_EXECUTE_CLEANUP_FUNCTION);
    pthread_exit(NULL);
}

int createPhilsThreads(pthread_t *threadsID, int countPhils, PhilTaskArg *threadArgs) {
    int retCode = SUCCESS;
    for (int i = 0; i < countPhils; ++i) {
        retCode = pthread_create(&threadsID[i], NULL, philTask, (void *) &threadArgs[i]);
        if (retCode != SUCCESS) {
            return retCode;
        }
    }
    return retCode;
}

int takeFood(Food *foodInfo, int *foodLeft) {
    int retCode = pthread_mutex_lock(&foodInfo->eatLock);
    if (retCode != SUCCESS) {
        return retCode;
    }
    if (foodInfo->foodLeft > 0) {
        --foodInfo->foodLeft;
    }
    *foodLeft = foodInfo->foodLeft;
    retCode = pthread_mutex_unlock(&foodInfo->eatLock);
    return retCode;
}

void cleanupPhilThreadsOnError(void *arg) {
    PhilTaskArg *taskArg = (PhilTaskArg *) arg;

    // Eat lock mutex unlock
    int retCode = pthread_mutex_unlock(&food.eatLock);
    printf("eatlock code: %d\n", retCode);
    if (retCode != SUCCESS && retCode != EPERM) {
        printPosixThreadError(pthread_self(), retCode, "Unlocking eat lock mutex on error");
        exit(EXIT_FAILURE);
    }

    // Left fork mutex unlock
    retCode = pthread_mutex_unlock(&forks[taskArg->leftForkMutexID]);
    if (retCode != SUCCESS && retCode != EPERM) {
        printPosixThreadError(pthread_self(), retCode, "Unlocking fork mutex on error");
        exit(EXIT_FAILURE);
    }

    // Right fork mutex unlock
    retCode = pthread_mutex_unlock(&forks[taskArg->rightForkMutexID]);
    if (retCode != SUCCESS && retCode != EPERM) {
        printPosixThreadError(pthread_self(), retCode, "Unlocking fork mutex on error");
        exit(EXIT_FAILURE);
    }
}

int getFork(int forkID, int philNum, char *philHand) {
    int retCode = pthread_mutex_lock(&forks[forkID]);
    if (retCode == SUCCESS) {
        printf("[Phil %d] Got %s fork %d\n", philNum, philHand, forkID);
    }
    return retCode;
}

int getForks(int philNumber, int leftForkNum, int rightForkNum) {
    int retCode;
    if (leftForkNum < rightForkNum) {
        retCode = getFork(rightForkNum, philNumber, "right");
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Getting right fork");
            return retCode;
        }
        retCode = getFork(leftForkNum, philNumber, "left");
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Getting left fork");
            return retCode;
        }
    } else {
        retCode = getFork(leftForkNum, philNumber, "left");
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Getting left fork");
            return retCode;
        }
        retCode = getFork(rightForkNum, philNumber, "right");
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Getting right fork");
            return retCode;
        }
    }
    return SUCCESS;
}

int downFork(int forkID, int philNum, char *philHand) {
    int retCode = pthread_mutex_unlock(&forks[forkID]);
    if (retCode == SUCCESS) {
        printf("[Phil %d] Down %s fork %d\n", philNum, philHand, forkID);
    }
    return retCode;
}

int downForks(int philNumber, int leftForkNum, int rightForkNum) {
    int retCode;
    if (leftForkNum > rightForkNum) {
        retCode = downFork(rightForkNum, philNumber, "right");
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Downing right fork");
            return retCode;
        }
        retCode = downFork(leftForkNum, philNumber, "left");
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Downing left fork");
            return retCode;
        }
    } else {
        retCode = downFork(leftForkNum, philNumber, "left");
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Downing left fork");
            return retCode;
        }
        retCode = downFork(rightForkNum, philNumber, "right");
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Downing right fork");
            return retCode;
        }
    }
    return SUCCESS;
}

int joinPhilsThreads(pthread_t *threadsID, int countPhils) {
    int retCode;
    for (int i = 0; i < countPhils; ++i) {
        retCode = pthread_join(threadsID[i], NULL);
        if (retCode != SUCCESS) {
            return retCode;
        }
    }
    return SUCCESS;
}

int cancelPhilsThreads() {
    int retCode = SUCCESS;
    for (int i = 0; i < PHIL_COUNT; ++i) {
        retCode = pthread_cancel(philsThID[i]);
        if (retCode != SUCCESS) {
            break;
        }
    }
    return retCode;
}
