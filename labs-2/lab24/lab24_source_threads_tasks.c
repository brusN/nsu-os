#include "lab24_source.h"

void widgetsThreadCleanupOnExit(void *arg) {
    if (cancelWidgetsFabricSubthreads() != SUCCESS) {
        // Error description printed by cancelWidgetsFabricSubthreads() function
        fprintf(stderr, "[Error] Error while canceling widgets fabric threads\n");
        exit(EXIT_FAILURE);
    }
    int retCode = printProductionResult();
    if (retCode != SUCCESS) {
        // Error description has printed by printProductionResult() function
        fprintf(stderr, "[Error] Got error while printing widgets semaphore value\n");
    }
    if (destroySemaphores() != SUCCESS) {
        // Error describe has printed by destroySemaphores() function
        fprintf(stderr, "[Error] Got error while destroying semaphores\n");
        exit(EXIT_FAILURE);
    }
}

void *detailAProducerTask(void *args) {
    int retCode = setThreadInitFlagValue(&threadsInfo.isDetailsAProducerThInit, THREAD_INITIALIZED);
    if (retCode != SUCCESS) {
        fprintf(stderr, "[Error] Error while setting threads info flag\n");
        pthread_exit(PTHREAD_CANCELED);
    }

    int producedDetailsA = 0;
    int requiredDetailsA = *(int *) args;
    while (producedDetailsA < requiredDetailsA) {
        sleep(DETAIL_A_PRODUCTION_TIME);
        ++producedDetailsA;
        if (sem_post(&semDetA) != SUCCESS) {
            printPosixThreadError(pthread_self(), errno, "Post semaphore of details A");
            int cancelThreadRetCode = pthread_cancel(widgetProducerThID);
            if (cancelThreadRetCode != SUCCESS) {
                printPosixThreadError(pthread_self(), cancelThreadRetCode, "Cancelling widget's fabric thread");
                exit(EXIT_FAILURE);
            }
        }
        printf("Created detail A\n");
    }

    retCode = setThreadInitFlagValue(&threadsInfo.isDetailsAProducerThInit, THREAD_NOT_INITIALIZED);
    if (retCode != SUCCESS) {
        fprintf(stderr, "[Error] Error while setting threads info flag\n");
        pthread_exit(PTHREAD_CANCELED);
    }
    pthread_exit(NULL);
}

void *detailBProducerTask(void *args) {
    int retCode = setThreadInitFlagValue(&threadsInfo.isDetailsBProducerThInit, THREAD_INITIALIZED);
    if (retCode != SUCCESS) {
        fprintf(stderr, "[Error] Error while setting threads info flag\n");
        pthread_exit(PTHREAD_CANCELED);
    }

    int producedDetailsB = 0;
    int requiredDetailsB = *(int *) args;
    while (producedDetailsB < requiredDetailsB) {
        sleep(DETAIL_B_PRODUCTION_TIME);
        ++producedDetailsB;
        if (sem_post(&semDetB) != SUCCESS) {
            printPosixThreadError(pthread_self(), errno, "Post semaphore of details B");
            int cancelThreadRetCode = pthread_cancel(widgetProducerThID);
            if (cancelThreadRetCode != SUCCESS) {
                printPosixThreadError(pthread_self(), cancelThreadRetCode, "Cancelling widget's fabric thread");
                exit(EXIT_FAILURE);
            }
        }

        printf("Created detail B\n");
    }

    retCode = setThreadInitFlagValue(&threadsInfo.isDetailsBProducerThInit, THREAD_NOT_INITIALIZED);
    if (retCode != SUCCESS) {
        fprintf(stderr, "[Error] Error while setting threads info flag\n");
        pthread_exit(PTHREAD_CANCELED);
    }
    pthread_exit(NULL);
}

void *detailCProducerTask(void *args) {
    int retCode = setThreadInitFlagValue(&threadsInfo.isDetailsCProducerThInit, THREAD_INITIALIZED);
    if (retCode != SUCCESS) {
        fprintf(stderr, "[Error] Error while setting threads info flag\n");
        pthread_exit(PTHREAD_CANCELED);
    }

    int producedDetailsC = 0;
    int requiredDetailsC = *(int *) args;
    while (producedDetailsC < requiredDetailsC) {
        sleep(DETAIL_C_PRODUCTION_TIME);
        ++producedDetailsC;
        if (sem_post(&semDetC) != SUCCESS) {
            printPosixThreadError(pthread_self(), errno, "Post semaphore of details C");
            int cancelThreadRetCode = pthread_cancel(widgetProducerThID);
            if (cancelThreadRetCode != SUCCESS) {
                printPosixThreadError(pthread_self(), cancelThreadRetCode, "Cancelling widget's fabric thread");
                exit(EXIT_FAILURE);
            }
        }
        printf("Created detail C\n");
    }

    retCode = setThreadInitFlagValue(&threadsInfo.isDetailsCProducerThInit, THREAD_NOT_INITIALIZED);
    if (retCode != SUCCESS) {
        fprintf(stderr, "[Error] Error while setting threads info flag\n");
        pthread_exit(PTHREAD_CANCELED);
    }
    pthread_exit(NULL);
}

void *moduleProducerTask(void *args) {
    int retCode = setThreadInitFlagValue(&threadsInfo.isModulesProducerThInit, THREAD_INITIALIZED);
    if (retCode != SUCCESS) {
        fprintf(stderr, "[Error] Error while setting threads info flag\n");
        pthread_exit(PTHREAD_CANCELED);
    }

    int countRequiredModules = *(int *) args;
    ModuleProducingInfo moduleProducingInfo;
    initModuleProducingInfo(&moduleProducingInfo, countRequiredModules);

    retCode = createModulesFabricThreads(&moduleProducingInfo);
    if (retCode != SUCCESS) {
        // Error description has printed by createModulesFabricThreads() function
        fprintf(stderr, "[Error] Error while creating details A and details B fabric threads\n");
        int cancelThreadRetCode = pthread_cancel(widgetProducerThID);
        if (cancelThreadRetCode != SUCCESS) {
            printPosixThreadError(pthread_self(), cancelThreadRetCode, "Cancelling widget's fabric thread");
            exit(EXIT_FAILURE);
        }
    }

    int curProducedModules = 0;
    while (curProducedModules < countRequiredModules) {
        if (sem_wait(&semDetA) != SUCCESS) {
            printPosixThreadError(pthread_self(), errno, "Waiting for semaphore of details A");
            int cancelThreadRetCode = pthread_cancel(widgetProducerThID);
            if (cancelThreadRetCode != SUCCESS) {
                printPosixThreadError(pthread_self(), cancelThreadRetCode, "Cancelling widget's fabric thread");
                exit(EXIT_FAILURE);
            }
        }
        if (sem_wait(&semDetB) != SUCCESS) {
            printPosixThreadError(pthread_self(), errno, "Waiting for semaphore of details B");
            int cancelThreadRetCode = pthread_cancel(widgetProducerThID);
            if (cancelThreadRetCode != SUCCESS) {
                printPosixThreadError(pthread_self(), cancelThreadRetCode, "Cancelling widget's fabric thread");
                exit(EXIT_FAILURE);
            }
        }
        ++curProducedModules;
        if (sem_post(&semModules) != SUCCESS) {
            printPosixThreadError(pthread_self(), errno, "Post semaphore of modules");
            int cancelThreadRetCode = pthread_cancel(widgetProducerThID);
            if (cancelThreadRetCode != SUCCESS) {
                printPosixThreadError(pthread_self(), cancelThreadRetCode, "Cancelling widget's fabric thread");
                exit(EXIT_FAILURE);
            }
        }
        printf("Created module\n");
    }

    if (joinModulesFabricThreads() != SUCCESS) {
        // Error description has printed by joinModulesFabricThreads() function
        fprintf(stderr, "[Error] Error while joining details A and details B fabric threads\n");
        exit(EXIT_FAILURE);
    }
    retCode = setThreadInitFlagValue(&threadsInfo.isModulesProducerThInit, THREAD_NOT_INITIALIZED);
    if (retCode != SUCCESS) {
        fprintf(stderr, "[Error] Error while setting threads info flag\n");
        pthread_exit(PTHREAD_CANCELED);
    }
    pthread_exit(NULL);
}

void *widgetProducerTask(void *arg) {
    pthread_cleanup_push(widgetsThreadCleanupOnExit, NULL)
            int retCode = setThreadInitFlagValue(&threadsInfo.isWidgetProducerThInit, THREAD_INITIALIZED);
            if (retCode != SUCCESS) {
                fprintf(stderr, "[Error] Error while setting threads info flag\n");
                pthread_exit(PTHREAD_CANCELED);
            }
            int countRequiredWidgets = *(int *) arg;
            WidgetProducingInfo widgetProducingInfo;
            initWidgetProducingInfo(&widgetProducingInfo, countRequiredWidgets);

            if (createWidgetsFabricThreads(&widgetProducingInfo) != SUCCESS) {
                // Error description printed by createWidgetsFabricThreads()
                fprintf(stderr, "[Error] Error while creating subthreads for widget's fabric\n");
                pthread_exit(PTHREAD_CANCELED);
            }

            // Producing widgets
            int curWidgetsCount = 0;
            while (curWidgetsCount < countRequiredWidgets) {
                if (sem_wait(&semDetC) != SUCCESS) {
                    printPosixThreadError(pthread_self(), errno, "Waiting for semaphore of details C");
                    pthread_exit(PTHREAD_CANCELED);
                }
                if (sem_wait(&semModules) != SUCCESS) {
                    printPosixThreadError(pthread_self(), errno, "Waiting for semaphore of modules");
                    pthread_exit(PTHREAD_CANCELED);
                }
                ++curWidgetsCount;
                if (sem_post(&semWidgets) != SUCCESS) {
                    printPosixThreadError(pthread_self(), errno, "Post semaphore of widgets");
                    pthread_exit(PTHREAD_CANCELED);
                }
                printf("Created widget\n");
            }

            if (joinWidgetsFabricThreads() != SUCCESS) {
                // Error description printed by joinWidgetsFabricThreads()
                fprintf(stderr, "[Error] Error while joining subthreads for widget's fabric\n");
                pthread_exit(PTHREAD_CANCELED);
            }
            retCode = setThreadInitFlagValue(&threadsInfo.isWidgetProducerThInit, THREAD_NOT_INITIALIZED);
            if (retCode != SUCCESS) {
                fprintf(stderr, "[Error] Error while setting threads info flag\n");
                pthread_exit(PTHREAD_CANCELED);
            }
    pthread_cleanup_pop(EXECUTE_CLEANUP_FUNCTION);
    pthread_exit(NULL);
}
