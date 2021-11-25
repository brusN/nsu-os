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
#define MAX_PRODUCES_WIDGET_COUNT 100
#define DETAIL_A_PRODUCTION_TIME 1
#define DETAIL_B_PRODUCTION_TIME 2
#define DETAIL_C_PRODUCTION_TIME 3

enum parseInputArgs_state {
    parseInputArgs_SUCCESS = 0,
    parseInputArgs_ILLEGAL_COUNT_INPUT_ARGS,
    parseInputArgs_OUT_OF_RANGE,
    parseInputArgs_NOT_A_NUMBER
};

sem_t semDetA, semDetB, semDetC, semModules, semWidgets;
pthread_t widgetProducerThID, detailAProducerThID, detailBProducerThID, detailCProducerThID, moduleProducerThID;

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

// Input args handling
int parseInputArguments(int argc, char **argv, int *countRequiredWidgets);

void printParseInputArgsError(int returnCode);

void initModuleProducingInfo(ModuleProducingInfo *moduleProducingInfo, int countRequiredModules);

void initWidgetProducingInfo(WidgetProducingInfo *widgetProducingInfo, int countRequiredWidgets);

// Signal handling
void signalHandler(int signum);

// Semaphores
void initSemaphores();

void destroySemaphores();

// Producer tasks
void *detailAProducerTask(void *arg);

void *detailBProducerTask(void *arg);

void *moduleProducerTask(void *arg);

void *detailCProducerTask(void *arg);

void *widgetProducerTask(void *arg);

void printProductionResult();

#endif //NSU_OS_LABS_LAB24_SOURCE_H
