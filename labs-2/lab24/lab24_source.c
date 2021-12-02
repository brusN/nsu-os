#include "lab24_source.h"

// ----------- Input arguments handling -----------

int parseInputArguments(int argc, char **argv, int *countRequiredWidgets) {
    // If program has launched without any arguments, then set default values
    if (argc == NO_INPUT_ARGS) {
        *countRequiredWidgets = MAX_COUNT_WIDGETS;
        return parseInputArgs_SUCCESS;
    }
    if (argc != REQUIRED_COUNT_INPUT_ARGS) {
        return parseInputArgs_ILLEGAL_COUNT_INPUT_ARGS;
    }
    long parsedRequiredCountWidgets;
    int returnCode = str2long(&parsedRequiredCountWidgets, argv[1]);
    if (returnCode == str2num_NOT_NUMBER || returnCode == str2num_EMPTY_STR) {
        return parseInputArgs_NOT_A_NUMBER;
    }
    if (returnCode == str2num_ERANGE || parsedRequiredCountWidgets > MAX_COUNT_WIDGETS ||
        parsedRequiredCountWidgets <= 0) {
        return parseInputArgs_OUT_OF_RANGE;
    }
    *countRequiredWidgets = (int) parsedRequiredCountWidgets;
    return parseInputArgs_SUCCESS;
}

void printParseInputArgsError(int returnCode) {
    switch (returnCode) {
        case parseInputArgs_NOT_A_NUMBER:
            fprintf(stderr, "[Error] Input argument not a number\n");
            break;
        case parseInputArgs_OUT_OF_RANGE:
            fprintf(stderr,
                    "[Error] Input argument out of range! Number must be positive and less than max count constraint %d\n",
                    MAX_COUNT_WIDGETS);
            break;
        case parseInputArgs_ILLEGAL_COUNT_INPUT_ARGS:
            fprintf(stderr,
                    "[Error] Illegal count arguments. Must be one or zero argument (sets default required count widgets)\n");
            break;
        default:
            fprintf(stderr, "[Error] Unknown error code\n");
            break;
    }
}

// ----------- Semaphores functions -----------

int initSemaphores() {
    if (sem_init(&semDetA, NO_PROCESS_SHARED, START_SEMAPHORE_VALUE) != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Init semaphore of detail A");
        return FAIL;
    }
    if (sem_init(&semDetB, NO_PROCESS_SHARED, START_SEMAPHORE_VALUE) != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Init semaphore of detail B");
        return FAIL;
    }
    if (sem_init(&semDetC, NO_PROCESS_SHARED, START_SEMAPHORE_VALUE) != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Init semaphore of detail C");
        return FAIL;
    }
    if (sem_init(&semModules, NO_PROCESS_SHARED, START_SEMAPHORE_VALUE) != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Init semaphore of modules");
        return FAIL;
    }
    if (sem_init(&semWidgets, NO_PROCESS_SHARED, START_SEMAPHORE_VALUE) != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Init semaphore of widgets");
        return FAIL;
    }
    return SUCCESS;
}

int destroySemaphores() {
    if (sem_destroy(&semDetA) != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Destroying semaphore of details A");
        return FAIL;
    }
    if (sem_destroy(&semDetB) != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Destroying semaphore of details B");
        return FAIL;
    }
    if (sem_destroy(&semDetC) != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Destroying semaphore of details C");
        return FAIL;
    }
    if (sem_destroy(&semModules) != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Destroying semaphore of modules");
        return FAIL;
    }
    if (sem_destroy(&semWidgets) != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Destroying semaphore of widgets");
        return FAIL;
    }
    return SUCCESS;
}

// ----------- Threads info functions -----------

int initMutex(pthread_mutex_t *mutex, int type, int robust) {
    // Creating ERRORCHECK and ROBUST mutex attribute
    pthread_mutexattr_t attr;
    int retCode = pthread_mutexattr_init(&attr);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Initializing threads info mutex attribute");
        return FAIL;
    }
    retCode = pthread_mutexattr_settype(&attr, type);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Setting type for threads info mutex");
        return FAIL;
    }
    retCode = pthread_mutexattr_setrobust(&attr, robust);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Setting robust for threads info mutex");
        int clearAttrRetCode = pthread_mutexattr_destroy(&attr);
        if (clearAttrRetCode != SUCCESS) {
            printPosixThreadError(pthread_self(), clearAttrRetCode, "Destroying threads info mutex attribute");
            exit(EXIT_FAILURE);
        }
        return FAIL;
    }

    // Initializing mutex
    retCode = pthread_mutex_init(mutex, &attr);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Creating mutex for threads info");
    }
    int clearAttrRetCode = pthread_mutexattr_destroy(&attr);
    if (clearAttrRetCode != SUCCESS) {
        printPosixThreadError(pthread_self(), clearAttrRetCode, "Destroying threads info mutex attribute");
        exit(EXIT_FAILURE);
    }
    return retCode;
}

int initThreadsInfo(ThreadsInfo *threadsInfoStruct) {
    threadsInfoStruct->isDetailsAProducerThInit = THREAD_NOT_INITIALIZED;
    threadsInfoStruct->isDetailsBProducerThInit = THREAD_NOT_INITIALIZED;
    threadsInfoStruct->isDetailsCProducerThInit = THREAD_NOT_INITIALIZED;
    threadsInfoStruct->isModulesProducerThInit = THREAD_NOT_INITIALIZED;
    threadsInfoStruct->isWidgetProducerThInit = THREAD_NOT_INITIALIZED;
    int retCode = initMutex(&threadsInfoStruct->infoLock, PTHREAD_MUTEX_ERRORCHECK, PTHREAD_MUTEX_ROBUST);
    return retCode;
}

int setThreadInitFlagValue(int *flag, int newValue) {
    int retCode = pthread_mutex_lock(&threadsInfo.infoLock);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Setting flag for widget producer thread");
        return FAIL;
    }
    *flag = newValue;
    retCode = pthread_mutex_unlock(&threadsInfo.infoLock);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Setting flag for widget producer thread");
        return FAIL;
    }
    return SUCCESS;
}

int destroyThreadsInfo(ThreadsInfo *threadsInfoStruct) {
    int retCode = pthread_mutex_destroy(&threadsInfoStruct->infoLock);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Destroying threads info mutex");
        return FAIL;
    }
    return SUCCESS;
}

// ----------- Producing threads functions -----------

void initModuleProducingInfo(ModuleProducingInfo *moduleProducingInfo, int countRequiredModules) {
    // Module = A + B
    moduleProducingInfo->countRequiredDetailsA = countRequiredModules;
    moduleProducingInfo->countRequiredDetailsB = countRequiredModules;
}

void initWidgetProducingInfo(WidgetProducingInfo *widgetProducingInfo, int countRequiredWidgets) {
    // Widget = Module + C
    widgetProducingInfo->countRequiredModules = countRequiredWidgets;
    widgetProducingInfo->countRequiredDetailsC = countRequiredWidgets;
}

int printProductionResult() {
    int totalProducedWidgets;
    if (sem_getvalue(&semWidgets, &totalProducedWidgets) != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Getting widget's semaphore value");
        return FAIL;
    }
    printf("Total widgets produced: %d\n", totalProducedWidgets);
    return SUCCESS;
}

int createWidgetsFabricThreads(WidgetProducingInfo *widgetProducingInfo) {
    int retCode = pthread_create(&moduleProducerThID, NULL, moduleProducerTask,
                                 (void *) &widgetProducingInfo->countRequiredModules);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Creating modules producer thread");
        return FAIL;
    }
    retCode = pthread_create(&detailCProducerThID, NULL, detailCProducerTask,
                             (void *) &widgetProducingInfo->countRequiredDetailsC);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Creating details C producer thread");
        int cancelThreadRetCode = pthread_cancel(moduleProducerThID);
        if (cancelThreadRetCode != SUCCESS) {
            printPosixThreadError(pthread_self(), cancelThreadRetCode, "Canceling modules producer on error");
            exit(EXIT_FAILURE);
        }
        return FAIL;
    }
    return SUCCESS;
}

int joinWidgetsFabricThreads() {
    int retCode = pthread_join(moduleProducerThID, NULL);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Joining module producer thread");
        return FAIL;
    }
    retCode = pthread_join(detailCProducerThID, NULL);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Joining details C producer thread");
        return FAIL;
    }

    return SUCCESS;
}

int cancelWidgetsFabricSubthreads() {
    int retCode = pthread_mutex_lock(&threadsInfo.infoLock);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Locking mutex for threads info struct");
        return FAIL;
    }
    if (threadsInfo.isDetailsAProducerThInit == THREAD_INITIALIZED) {
        retCode = pthread_cancel(detailAProducerThID);
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Canceling detail A producer thread");
            return FAIL;
        }
    }
    if (threadsInfo.isDetailsBProducerThInit == THREAD_INITIALIZED) {
        retCode = pthread_cancel(detailBProducerThID);
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Canceling detail B producer thread");
            return FAIL;
        }
    }
    if (threadsInfo.isDetailsCProducerThInit == THREAD_INITIALIZED) {
        retCode = pthread_cancel(detailCProducerThID);
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Canceling detail C producer thread");
            return FAIL;
        }
    }
    if (threadsInfo.isModulesProducerThInit == THREAD_INITIALIZED) {
        retCode = pthread_cancel(moduleProducerThID);
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Canceling modules producer thread");
            return FAIL;
        }
    }
    retCode = pthread_mutex_unlock(&threadsInfo.infoLock);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Unlocking mutex for threads info struct");
        return FAIL;
    }
    return SUCCESS;
}

int createModulesFabricThreads(ModuleProducingInfo *moduleProducingInfo) {
    int retCode = pthread_create(&detailAProducerThID, NULL, detailAProducerTask,
                                 &moduleProducingInfo->countRequiredDetailsA);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Creating detail A fabric thread");
        return FAIL;
    }
    retCode = pthread_create(&detailBProducerThID, NULL, detailBProducerTask,
                             &moduleProducingInfo->countRequiredDetailsB);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Creating detail B fabric thread");
        int cancelThreadRetCode = pthread_cancel(detailAProducerThID);
        if (cancelThreadRetCode != SUCCESS) {
            printPosixThreadError(pthread_self(), cancelThreadRetCode, "Cancelling detail A fabric thread on error");
            exit(EXIT_FAILURE);
        }
        return FAIL;
    }
    return SUCCESS;
}

int joinModulesFabricThreads() {
    int retCode = pthread_join(detailAProducerThID, NULL);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Joining detail A thread");
        return FAIL;
    }
    retCode = pthread_join(detailBProducerThID, NULL);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Joining detail B thread");
        return FAIL;
    }
    return SUCCESS;
}

int produceWidgets(int requiredCountWidgets) {
    int retCode = pthread_create(&widgetProducerThID, NULL, widgetProducerTask, (void *) &requiredCountWidgets);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Creating widgets thread");
        if (destroySemaphores() != SUCCESS) {
            // Error describe has printed by destroySemaphores() function
            fprintf(stderr, "[Error] Got error while destroying semaphores\n");
            exit(EXIT_FAILURE);
        }
        return FAIL;
    }
    printf("Widget's fabric has started!\n");
    retCode = pthread_join(widgetProducerThID, NULL);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Joining widgets thread");
        if (destroySemaphores() != SUCCESS) {
            // Error describe has printed by destroySemaphores() function
            fprintf(stderr, "[Error] Got error while destroying semaphores\n");
            exit(EXIT_FAILURE);
        }
        return FAIL;
    }
    return SUCCESS;
}
