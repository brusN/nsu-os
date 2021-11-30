#include "lab24_source.h"

int parseInputArguments(int argc, char **argv, int *countRequiredWidgets) {
    if (argc == NO_INPUT_ARGS) {
        *countRequiredWidgets = MAX_PRODUCES_WIDGET_COUNT;
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

    if (returnCode == str2num_ERANGE || parsedRequiredCountWidgets > MAX_PRODUCES_WIDGET_COUNT ||
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
                    MAX_PRODUCES_WIDGET_COUNT);
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

void initModuleProducingInfo(ModuleProducingInfo *moduleProducingInfo, int countRequiredModules) {
    moduleProducingInfo->countRequiredDetailsA = countRequiredModules;
    moduleProducingInfo->countRequiredDetailsB = countRequiredModules;
}

void initWidgetProducingInfo(WidgetProducingInfo *widgetProducingInfo, int countRequiredWidgets) {
    widgetProducingInfo->countRequiredModules = countRequiredWidgets;
    widgetProducingInfo->countRequiredDetailsC = countRequiredWidgets;
}

int printProductionResult() {
    int totalProducedWidgets = 0;
    if (sem_getvalue(&semWidgets, &totalProducedWidgets) != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Getting value of widgets semaphore");
        return FAIL;
    }

    // Two cases of stopping producing: reach producing limit or canceling by user
    if (totalProducedWidgets == MAX_PRODUCES_WIDGET_COUNT) {
        printf("Got max count produced widgets: %d\n", MAX_PRODUCES_WIDGET_COUNT);
    } else {
        printf("Total widgets produced: %d\n", totalProducedWidgets);
    }
    return SUCCESS;
}

void signalHandler(int signum) {
    // Cancel production threads which required for widgets
    int retCode = pthread_cancel(moduleProducerThID);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Canceling module producer thread");
        exit(EXIT_FAILURE);
    }
    retCode = pthread_cancel(detailCProducerThID);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Canceling detail C producer thread");
        exit(EXIT_FAILURE);
    }

    printf("Fabric has stopped!\n");
    printProductionResult();
    destroySemaphores();
    exit(EXIT_SUCCESS);
}

void *widgetProducerTask(void *arg) {
    int countRequiredWidgets = *(int *) arg;
    WidgetProducingInfo widgetProducingInfo;
    initWidgetProducingInfo(&widgetProducingInfo, countRequiredWidgets);

    int retCode = pthread_create(&moduleProducerThID, NULL, moduleProducerTask, (void *) &widgetProducingInfo.countRequiredModules);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Creating modules producer thread");
        pthread_exit(PTHREAD_CANCELED);
    }
    retCode = pthread_create(&detailCProducerThID, NULL, detailCProducerTask, (void *) &widgetProducingInfo.countRequiredDetailsC);
    if (retCode != SUCCESS) {
        printPosixThreadError(pthread_self(), retCode, "Creating details C producer thread");
        retCode = pthread_cancel(moduleProducerThID);
        if (retCode != SUCCESS) {
            printPosixThreadError(pthread_self(), retCode, "Canceling modules producer on error");
            exit(EXIT_FAILURE);
        }
        pthread_exit(PTHREAD_CANCELED);
    }

    // Producing widgets
    int curWidgetsCount = 0;
    while (curWidgetsCount < countRequiredWidgets) {
        sem_wait(&semDetC);
        sem_wait(&semModules);
        ++curWidgetsCount;
        sem_post(&semWidgets);
        printf("Created widget\n");
        if (curWidgetsCount == MAX_PRODUCES_WIDGET_COUNT) {
            kill(getpid(), SIGINT);
        }
    }

    pthread_join(moduleProducerThID, NULL);
    pthread_join(detailCProducerThID, NULL);
    pthread_exit(NULL);
}

void *detailAProducerTask(void *args) {
    int producedDetailsA = 0;
    int requiredDetailsA = *(int *) args;
    while (producedDetailsA < requiredDetailsA) {
        sleep(DETAIL_A_PRODUCTION_TIME);
        ++producedDetailsA;
        sem_post(&semDetA);
        printf("Created detail A\n");
    }
    pthread_exit(NULL);
}

void *detailBProducerTask(void *args) {
    int producedDetailsB = 0;
    int requiredDetailsB = *(int *) args;
    while (producedDetailsB < requiredDetailsB) {
        sleep(DETAIL_B_PRODUCTION_TIME);
        ++producedDetailsB;
        validateFuncResult(sem_post(&semDetB), "Post for details B semaphore");
        printf("Created detail B\n");
    }
    pthread_exit(NULL);
}

void *detailCProducerTask(void *args) {
    int producedDetailsC = 0;
    int requiredDetailsC = *(int *) args;
    while (producedDetailsC < requiredDetailsC) {
        sleep(DETAIL_C_PRODUCTION_TIME);
        ++producedDetailsC;
        sem_post(&semDetC);
        printf("Created detail C\n");
    }
    pthread_exit(NULL);
}

void *moduleProducerTask(void *args) {
    int countRequiredModules = *(int *) args;
    ModuleProducingInfo moduleProducingInfo;
    initModuleProducingInfo(&moduleProducingInfo, countRequiredModules);
    validatePosixThreadFuncResult(
            pthread_create(&detailAProducerThID, NULL, detailAProducerTask, &moduleProducingInfo.countRequiredDetailsA),
            "Creating details A producer thread");
    validatePosixThreadFuncResult(
            pthread_create(&detailBProducerThID, NULL, detailBProducerTask, &moduleProducingInfo.countRequiredDetailsB),
            "Creating details B producer thread");

    int curProducedModules = 0;
    while (curProducedModules < countRequiredModules) {
        validateFuncResult(sem_wait(&semDetA), "Waiting for details A semaphore");
        validateFuncResult(sem_wait(&semDetB), "Waiting for details B semaphore");
        ++curProducedModules;
        validateFuncResult(sem_post(&semModules), "Post for modules semaphore");
        printf("Created module\n");
    }

    validatePosixThreadFuncResult(pthread_join(detailAProducerThID, NULL), "Joining details A producer thread");
    validatePosixThreadFuncResult(pthread_join(detailBProducerThID, NULL), "Joining details B producer thread");
    pthread_exit(NULL);
}

int initSemaphores() {
    if (sem_init(&semDetA, 0, 0) != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Init semaphore of detail A");
    }
    if (sem_init(&semDetB, 0, 0) != SUCCESS) {
        printPosixThreadError(pthread_self(), errno, "Init semaphore of detail B");
    }
    sem_init(&semDetC, 0, 0);
    sem_init(&semModules, 0, 0)
    sem_init(&semWidgets, 0, 0)
}

void destroySemaphores() {
    sem_destroy(&semDetA);
    sem_destroy(&semDetB);
    sem_destroy(&semDetC);
    sem_destroy(&semModules)
    sem_destroy(&semWidgets)
}

int startWidgetsFactory(WidgetProducingInfo *producingInfo) {

}