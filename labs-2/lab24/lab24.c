#include <stdio.h>
#include <unistd.h>
#include "lab24_source.h"

int main() {
    signal(SIGINT, sigintHandler);
    WidgetFactoryInfo info;
    initWidgetFactoryInfo(&info);

    return EXIT_SUCCESS;
}   
