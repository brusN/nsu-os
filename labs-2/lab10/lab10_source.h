#ifndef NSU_OS_LABS_LAB10_SOURCE_H
#define NSU_OS_LABS_LAB10_SOURCE_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "error_handling.h"

#define SUCCESS 0
#define FAIL 1
#define PHIL_COUNT 5
#define DELAY 30000 // in ns
#define THINK_TIME 1e6 // ~ 1 sec in ns
#define FOOD_INIT_COUNT 20
#define RESOURCE_NOT_INITIALIZED 0
#define RESOURCE_INITIALIZED 1
#define FALSE 0
#define TRUE 1
#define EXECUTE_CLEANUP_FUNCTION 1
#define DO_NOT_EXECUTE_CLEANUP_FUNCTION 0

typedef struct st_ResourcesInfo ResourcesInfo;
struct st_ResourcesInfo {
    int isFoodMutexInitialized;
    int isForksMutexesInitialized;
};

typedef struct st_Food Food;
struct st_Food {
    pthread_mutex_t eatLock;
    int foodLeft;
};

typedef struct st_PhilTaskArg PhilTaskArg;
struct st_PhilTaskArg {
    int philNumber;
    Food *food;
    int leftForkMutexID;
    int rightForkMutexID;
};

Food food;
pthread_t philsThID[PHIL_COUNT];
pthread_mutex_t forks[PHIL_COUNT];

// Clean resources functions
void initResourcesInfo(ResourcesInfo *resourcesInfo);
int cleanResources(ResourcesInfo *resourcesInfo);

// For forks mutexes
int initMutexesAttribute(pthread_mutexattr_t *attr, int mutexType, int mutexRobust);
int initMutexes(pthread_mutex_t *mutexes, int count);
int destroyMutexes(pthread_mutex_t *mutexes, int count);

// Philosopher tasks
int initFood(Food *foodInfo, int initCount);
void initPhilTaskArgs(PhilTaskArg *taskArgs, int countPhils, Food *foodInfo);
void *philTask(void *arg);
int createPhilsThreads(pthread_t *threadsID, int countPhils, PhilTaskArg *threadArgs);
int joinPhilsThreads(pthread_t *threadsID, int countPhils);

// Philosopher actions
int takeFood(Food *foodInfo, int *foodLeft);
int getFork(int forkID, int philNum, char *philHand);
int getForks(int philNumber, int leftForkNum, int rightForkNum);
int downFork(int forkID, int philNum, char *philHand);
int downForks(int philNumber, int leftForkNum, int rightForkNum);

// Error handling in philosophers tasks
int cancelPhilsThreads();
void cleanupPhilThreadsOnError(void *arg);

#endif //NSU_OS_LABS_LAB10_SOURCE_H
