#include "lab10_source.h"

void initFood(Food *food) {
    food->foodLeft = START_FOOD;
    int returnCode = pthread_mutex_init(&food->eatLock, NULL);
    if (returnCode != SUCCESS) {
        printPosixThreadError(pthread_self(), returnCode);
        exit(EXIT_FAILURE);
    }
}

void createPhilTasks(PhilTaskArg *taskArgs, int countPhils, Food *food, pthread_mutex_t *forks) {
    for (int i = 0; i < countPhils; ++i) {
        taskArgs[i].philNumber = i;
        taskArgs[i].food = food;
        taskArgs[i].forks = forks;
        taskArgs[i].leftForkMutexID = i;
        taskArgs[i].rightForkMutexID = (i + 1 == PHIL_COUNT ? 0 : i + 1);
    }
}

int takeFood(Food *food) {
    int returnCode = pthread_mutex_lock(&food->eatLock);
    if (returnCode != SUCCESS) {
        printPosixThreadError(pthread_self(), returnCode);
        exit(EXIT_FAILURE);
    }

    if (food->foodLeft > 0) {
        --food->foodLeft;
    }

    returnCode = pthread_mutex_unlock(&food->eatLock);
    if (returnCode != SUCCESS) {
        printPosixThreadError(pthread_self(), returnCode);
        exit(EXIT_FAILURE);
    }
    return food->foodLeft;
}

void getForks(int philNumber, pthread_mutex_t *forks, int leftForkNum, int rightForkNum) {
    if (leftForkNum < rightForkNum) {
        getFork(forks, rightForkNum, philNumber, "right");
        getFork(forks, leftForkNum, philNumber,  "left");
    } else {
        getFork(forks, leftForkNum, philNumber,  "left");
        getFork(forks, rightForkNum, philNumber, "right");
    }
}

void getFork(pthread_mutex_t *forks, int forkID, int philNum, char *philHand) {
    int returnCode = pthread_mutex_lock(&forks[forkID]);
    if (returnCode != SUCCESS) {
        printPosixThreadError(pthread_self(), returnCode);
        exit(EXIT_FAILURE);
    }

    printf("[Phil %d] Get %s fork %d\n", philNum, philHand, forkID);
}

void downFork(pthread_mutex_t *forks, int forkID, int philNum, char *philHand) {
    int returnCode = pthread_mutex_unlock(&forks[forkID]);
    if (returnCode != SUCCESS) {
        printPosixThreadError(pthread_self(), returnCode);
        exit(EXIT_FAILURE);
    }

    printf("[Phil %d] Down %s fork %d\n", philNum, philHand, forkID);
}

void downForks(int philNumber, pthread_mutex_t *forks, int leftForkNum, int rightForkNum) {
    if (rightForkNum < leftForkNum) {
        downFork(forks, rightForkNum, philNumber, "right");
        downFork(forks, leftForkNum, philNumber,  "left");
    } else {
        downFork(forks, leftForkNum, philNumber,  "left");
        downFork(forks, rightForkNum, philNumber, "right");
    }
}

void *philTask(void *arg) {
    PhilTaskArg *taskArg = (PhilTaskArg *) arg;
    printf("Philosopher %d sitting down to dinner.\n", taskArg->philNumber);

    int food = takeFood(taskArg->food);
    while (food) {
        printf("[Phil %d] Get dish %d.\n", taskArg->philNumber, food);
        getForks(taskArg->philNumber, taskArg->forks, taskArg->leftForkMutexID, taskArg->rightForkMutexID);

        printf("[Phil %d] Eating.\n", taskArg->philNumber);
        usleep(DELAY * (START_FOOD - food + 1));
        downForks(taskArg->philNumber, taskArg->forks, taskArg->leftForkMutexID, taskArg->rightForkMutexID);
        food = takeFood(taskArg->food);
    }
    printf("[Phil %d] Done eating.\n", taskArg->philNumber);
    pthread_exit(NULL);
}


