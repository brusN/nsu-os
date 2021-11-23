#include "lab24_source.h"

void sigintHandler(int signum) {
    printf("Received SIGINT signal\nStopping factory...");
    exit(EXIT_SUCCESS);
}

void initWidgetFactoryInfo(WidgetFactoryInfo *info) {
    info->countDetailsA = 0;
    info->countDetailsB = 0;
    info->countDetailsModule = 0;
    info->countDetailsC = 0;
    info->countWidgets = 0;
}