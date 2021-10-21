#include <stdio.h>
#include "lab8_source.h"

int main(int argc, char **argv) {
    // Parsing and validating input arguments. Both args must be positive numbers
    inputArgs inputArgsValues;
    int retCode = parseInputArgs(argc, argv, &inputArgsValues);
    if (retCode != parseInputArgs_SUCCESS) {
        invalidInputArgsExit(retCode);
    }
    printf("Number of threads: %d\n", inputArgsValues.numThreads);
    printf("Number of iterations: %d\n", inputArgsValues.numIterations);

    // Calculating Pi
    double pi;
    ThreadErrorState state = calcPi(inputArgsValues.numThreads, inputArgsValues.numIterations, &pi);
    if (!isThreadErrStateSuccess(state)) {
        threadErrorExit(state);
    }

    printf("Pi number has been calculated: %.15g\n", pi);
    return EXIT_SUCCESS;
}