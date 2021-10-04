#include <stdio.h>
#include "lab8_source.h"


int main(int argc, char **argv) {
    int numThreads = 0;
    parseInputArguments(argc, argv, &numThreads);
    printf("Number of threads: %d\n", numThreads);

    threadFuncArg *args = createThreadsFunctionArgs(numThreads, NUM_ITERATIONS);
    pthread_t *threadID = createThreadsForTask(numThreads, threadTask, args);
    double pi = collectResults(threadID, numThreads, args) * 4.0;
    printf("Pi number has been calculated: %.15g\n", pi);

    free(threadID);
    free(args);
    return EXIT_SUCCESS;
}