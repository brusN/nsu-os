#ifndef NSU_OS_LABS_LAB24_SOURCE_H
#define NSU_OS_LABS_LAB24_SOURCE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include "mylib.h"
#include "error_handling.h"

#define NO_INPUT_ARGS 1
#define REQUIRED_COUNT_INPUT_ARGS 2
#define SUCCESS 0
#define FAIL 1
#define MAX_COUNT_WIDGETS 10
#define DETAIL_A_PRODUCTION_TIME 1
#define DETAIL_B_PRODUCTION_TIME 2
#define DETAIL_C_PRODUCTION_TIME 3
#define NO_PROCESS_SHARED 1
#define PROCESS_SHARED 0
#define START_SEMAPHORE_VALUE 0
#define EXECUTE_CLEANUP_FUNCTION 1
#define DO_NOT_EXECUTE_CLEANUP_FUNCTION 0
#define THREAD_NOT_INITIALIZED 0
#define THREAD_INITIALIZED 1

enum parseInputArgs_state {
    parseInputArgs_SUCCESS = 0,
    parseInputArgs_ILLEGAL_COUNT_INPUT_ARGS,
    parseInputArgs_OUT_OF_RANGE,
    parseInputArgs_NOT_A_NUMBER
};

typedef struct st_ThreadsInfo ThreadsInfo;
struct st_ThreadsInfo {
    pthread_mutex_t infoLock;
    int isWidgetProducerThInit;
    int isModulesProducerThInit;
    int isDetailsCProducerThInit;
    int isDetailsAProducerThInit;
    int isDetailsBProducerThInit;
};

typedef struct st_ModuleProducingInfo ModuleProducingInfo;
struct st_ModuleProducingInfo {
    int countRequiredDetailsA;
    int countRequiredDetailsB;
};
typedef struct st_WidgetProducingInfo WidgetProducingInfo;
struct st_WidgetProducingInfo {
    int countRequiredModules;
    int countRequiredDetailsC;
};

sem_t semDetA, semDetB, semDetC, semModules, semWidgets;
pthread_t widgetProducerThID, detailAProducerThID, detailBProducerThID, detailCProducerThID, moduleProducerThID;
ThreadsInfo threadsInfo;

// Threads info operations
int initMutex(pthread_mutex_t *mutex, int type, int robust);
int initThreadsInfo(ThreadsInfo *threadsInfoStruct);
int setThreadInitFlagValue(int *flag, int newValue);
int destroyThreadsInfo(ThreadsInfo *threadsInfoStruct);

// Input args handling
int parseInputArguments(int argc, char **argv, int *countRequiredWidgets);
void printParseInputArgsError(int returnCode);

// Semaphores
int initSemaphores();
int destroySemaphores();

// Widgets fabric
int produceWidgets(int requiredCountWidgets);
void widgetsThreadCleanupOnExit(void *arg);
void producerThreadCleanupOnExit(void *arg);
void initWidgetProducingInfo(WidgetProducingInfo *widgetProducingInfo, int countRequiredWidgets);
int createWidgetsFabricThreads(WidgetProducingInfo *widgetProducingInfo);
int joinWidgetsFabricThreads();

void initModuleProducingInfo(ModuleProducingInfo *moduleProducingInfo, int countRequiredModules);
int createModulesFabricThreads(ModuleProducingInfo *moduleProducingInfo);
int joinModulesFabricThreads();

int cancelWidgetsFabricSubthreads();
int printProductionResult();

// Producer tasks
void *detailAProducerTask(void *arg);
void *detailBProducerTask(void *arg);
void *moduleProducerTask(void *arg);
void *detailCProducerTask(void *arg);
void *widgetProducerTask(void *arg);

#endif //NSU_OS_LABS_LAB24_SOURCE_H
