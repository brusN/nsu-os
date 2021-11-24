#include "lab24_source.h"

void initModuleProducingInfo(ModuleProducingInfo *moduleProducingInfo, int countRequiredModules) {
    moduleProducingInfo->countRequiredDetailsA = countRequiredModules;
    moduleProducingInfo->countRequiredDetailsB = countRequiredModules;
}

void initWidgetProducingInfo(WidgetProducingInfo *widgetProducingInfo, int countRequiredWidgets) {
    widgetProducingInfo->countRequiredModules = countRequiredWidgets;
    widgetProducingInfo->countRequiredDetailsC = countRequiredWidgets;
}

void printProductionResult() {
    int totalProducedWidgets = 0;
    validateFuncResult(sem_getvalue(&semWidgets, &totalProducedWidgets), "Getting value of widgets semaphore");

    // Two cases of stopping producing: reach producing limit or canceling by user
    if (totalProducedWidgets == MAX_PRODUCES_WIDGET_COUNT) {
        printf("Got max count produced widgets: %d\n", MAX_PRODUCES_WIDGET_COUNT);
    } else {
        printf("Total widgets produced: %d\n", totalProducedWidgets);
    }
}

void signalHandler(int signum) {
    // Cancel production threads which required for widgets
    validatePosixThreadFuncResult(pthread_cancel(moduleProducerThID), "Canceling producing modules thread");
    validatePosixThreadFuncResult(pthread_cancel(detailCProducerThID), "Canceling producing details C thread");
    printf("Fabric has stopped!\n");
    printProductionResult();
    destroySemaphores();
    exit(EXIT_SUCCESS);
}

void *widgetProducerTask(void *arg) {
    int countRequiredWidgets = *(int *)arg;
    WidgetProducingInfo widgetProducingInfo;
    initWidgetProducingInfo(&widgetProducingInfo, countRequiredWidgets);

    // Creating detail producers
    validatePosixThreadFuncResult(pthread_create(&moduleProducerThID, NULL, moduleProducerTask, (void *)&widgetProducingInfo.countRequiredModules), "Creating modules producer thread");
    validatePosixThreadFuncResult(pthread_create(&detailCProducerThID, NULL, detailCProducerTask, (void *)&widgetProducingInfo.countRequiredDetailsC), "Creating details C producer thread");

    // Producing widgets
    int curWidgetsCount = 0;
    while (curWidgetsCount < countRequiredWidgets) {
        validateFuncResult(sem_wait(&semDetC), "Waiting for details C semaphore");
        validateFuncResult(sem_wait(&semModules), "Waiting for modules semaphore");
        ++curWidgetsCount;
        validateFuncResult(sem_post(&semWidgets), "Post for widgets semaphore");
        printf("Created widget\n");
        if (curWidgetsCount == MAX_PRODUCES_WIDGET_COUNT) {
            kill(getpid(), SIGINT);
        }
    }

    validatePosixThreadFuncResult(pthread_join(moduleProducerThID, NULL), "Joining modules producer thread");
    validatePosixThreadFuncResult(pthread_join(detailCProducerThID, NULL), "Joining details C producer thread");
    pthread_exit(NULL);
}

void *detailAProducerTask(void *args) {
    int producedDetailsA = 0;
    int requiredDetailsA = *(int *)args;
    while (producedDetailsA < requiredDetailsA) {
        sleep(DETAIL_A_PRODUCTION_TIME);
        ++producedDetailsA;
        validateFuncResult(sem_post(&semDetA), "Post for details A semaphore");
        printf("Created detail A\n");
    }
    pthread_exit(NULL);
}

void *detailBProducerTask(void *args) {
    int producedDetailsB = 0;
    int requiredDetailsB = *(int *)args;
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
    int requiredDetailsC = *(int *)args;
    while (producedDetailsC < requiredDetailsC) {
        sleep(DETAIL_C_PRODUCTION_TIME);
        ++producedDetailsC;
        validateFuncResult(sem_post(&semDetC), "Post for details C semaphore");
        printf("Created detail C\n");
    }
    pthread_exit(NULL);
}

void *moduleProducerTask(void *args) {
    int countRequiredModules = *(int *)args;
    ModuleProducingInfo moduleProducingInfo;
    initModuleProducingInfo(&moduleProducingInfo, countRequiredModules);
    validatePosixThreadFuncResult(pthread_create(&detailAProducerThID, NULL, detailAProducerTask, &moduleProducingInfo.countRequiredDetailsA), "Creating details A producer thread");
    validatePosixThreadFuncResult(pthread_create(&detailBProducerThID, NULL, detailBProducerTask, &moduleProducingInfo.countRequiredDetailsB), "Creating details B producer thread");

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

void initSemaphores() {
    validateFuncResult(sem_init(&semDetA, 0, 0), "Init details A semaphore");
    validateFuncResult(sem_init(&semDetB, 0, 0), "Init details A semaphore");
    validateFuncResult(sem_init(&semDetC, 0, 0), "Init details A semaphore");
    validateFuncResult(sem_init(&semModules, 0, 0), "Init modules semaphore");
    validateFuncResult(sem_init(&semWidgets, 0, 0), "Init widgets semaphore");
}

void destroySemaphores() {
    validateFuncResult(sem_destroy(&semDetA), "Destroy details A semaphore");
    validateFuncResult(sem_destroy(&semDetB), "Destroy details B semaphore");
    validateFuncResult(sem_destroy(&semDetC), "Destroy details C semaphore");
    validateFuncResult(sem_destroy(&semModules), "Destroy modules semaphore");
    validateFuncResult(sem_destroy(&semWidgets), "Destroy widgets semaphore");
}