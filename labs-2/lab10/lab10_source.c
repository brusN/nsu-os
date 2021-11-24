#include "lab10_source.h"

int initMutexes() {
    int returnCode = 0;
    for (int i = 0; i < PHIL_COUNT; ++i) {
        returnCode = pthread_mutex_init(&forks[i], NULL);
        if (returnCode != SUCCESS) {
            break;
        }
    }
    return returnCode;
}

int initFood(Food *food) {
    food->foodLeft = START_FOOD;
    return pthread_mutex_init(&food->eatLock, NULL);
}

void createPhilTasks(PhilTaskArg *taskArgs, int countPhils, Food *food) {
    for (int i = 0; i < countPhils; ++i) {
        taskArgs[i].philNumber = i;
        taskArgs[i].food = food;
        taskArgs[i].leftForkMutexID = i;
        taskArgs[i].rightForkMutexID = (i + 1 == PHIL_COUNT ? 0 : i + 1);
    }
}

int takeFood(Food *food) {
    validatePosixThreadFuncResult(pthread_mutex_lock(&food->eatLock), "Locking mutex for taking food");
    if (food->foodLeft > 0) {
        --food->foodLeft;
    }
    validatePosixThreadFuncResult(pthread_mutex_unlock(&food->eatLock), "Locking mutex for taking food");
    return food->foodLeft;
}

void getForks(int philNumber, int leftForkNum, int rightForkNum) {
    if (leftForkNum < rightForkNum) {
        getFork(rightForkNum, philNumber, "right");
        getFork(leftForkNum, philNumber,  "left");
    } else {
        getFork(leftForkNum, philNumber,  "left");
        getFork(rightForkNum, philNumber, "right");
    }
}

void getFork(int forkID, int philNum, char *philHand) {
    validatePosixThreadFuncResult(pthread_mutex_lock(&forks[forkID]), "Locking mutex for getting fork");
    printf("[Phil %d] Get %s fork %d\n", philNum, philHand, forkID);
}

void downFork(int forkID, int philNum, char *philHand) {
    validatePosixThreadFuncResult(pthread_mutex_unlock(&forks[forkID]), "Locking mutex for downing fork");
    printf("[Phil %d] Down %s fork %d\n", philNum, philHand, forkID);
}

void downForks(int philNumber, int leftForkNum, int rightForkNum) {
    if (leftForkNum > rightForkNum) {
        downFork(rightForkNum, philNumber, "right");
        downFork(leftForkNum, philNumber,  "left");
    } else {
        downFork(leftForkNum, philNumber,  "left");
        downFork(rightForkNum, philNumber, "right");
    }
}

void *philTask(void *arg) {
    PhilTaskArg *taskArg = (PhilTaskArg *) arg;
    printf("Philosopher %d sitting down to dinner.\n", taskArg->philNumber);

    int food = takeFood(taskArg->food);
    while (food) {
        printf("[Phil %d] Thinking...\n", taskArg->philNumber);
        usleep(THINK_TIME); // Thinking
        printf("[Phil %d] Getting dish %d\n", taskArg->philNumber, food);
        getForks(taskArg->philNumber, taskArg->leftForkMutexID, taskArg->rightForkMutexID);
        printf("[Phil %d] Eating\n", taskArg->philNumber);
        usleep(DELAY * (START_FOOD - food + 1)); // Eating
        downForks(taskArg->philNumber, taskArg->leftForkMutexID, taskArg->rightForkMutexID);
        food = takeFood(taskArg->food);
    }
    printf("[Phil %d] Done eating\n", taskArg->philNumber);
    pthread_exit(NULL);
}

int destroyMutexes() {
    int returnCode = 0;
    for (int i = 0; i < PHIL_COUNT; ++i) {
        returnCode = pthread_mutex_destroy(&forks[i]);
        if (returnCode != SUCCESS) {
            break;
        }
    }
    return returnCode;
}


