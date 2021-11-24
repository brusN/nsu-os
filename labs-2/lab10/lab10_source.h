#ifndef NSU_OS_LABS_LAB10_SOURCE_H
#define NSU_OS_LABS_LAB10_SOURCE_H

#include <unistd.h>
#include <pthread.h>
#include "mylib.h"
#include "limits.h"
#include "error_handling.h"


#define PHIL_COUNT 5
#define DELAY 30000
#define START_FOOD 50
#define THINK_TIME 1000000

#define SUCCESS 0
#define FAIL 1

pthread_mutex_t forks[PHIL_COUNT];

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

int initFood(Food *food);
int initMutexes();
void createPhilTasks(PhilTaskArg *taskArgs, int countPhils, Food *food);
void *philTask(void *arg);
int takeFood(Food *food);
void getFork(int forkID, int philNum, char *philHand);
void getForks(int philNumber, int leftForkNum, int rightForkNum);
void downFork(int forkID, int philNum, char *philHand);
void downForks(int philNumber, int leftForkNum, int rightForkNum);
int destroyMutexes();

#endif //NSU_OS_LABS_LAB10_SOURCE_H
