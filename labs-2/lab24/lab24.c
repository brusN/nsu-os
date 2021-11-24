#include "lab24_source.h"

int main() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    return EXIT_SUCCESS;
}   
