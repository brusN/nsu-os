#ifndef NSU_OS_LABS_LAB24_SOURCE_H
#define NSU_OS_LABS_LAB24_SOURCE_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>

#define SUCCESS 0
#define FAIL 1
#define MAX_WIDGET_COUNT 200
#define DETAIL_A_PRODUCTION_TIME 1
#define DETAIL_B_PRODUCTION_TIME 2
#define DETAIL_C_PRODUCTION_TIME 3

typedef struct st_widgetFactoryInfo WidgetFactoryInfo;
struct st_widgetFactoryInfo {
    int *countDetailsA;
    int *countDetailsB;
    int *countDetailsModule;
    int *countDetailsC;
    int *countWidgets;
};

void sigintHandler(int signum);
void initWidgetFactoryInfo(WidgetFactoryInfo *info);
void initSemaphores();

#endif //NSU_OS_LABS_LAB24_SOURCE_H
