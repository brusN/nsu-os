#ifndef LAB1_OS_COMMANDS_H
#define LAB1_OS_COMMANDS_H

#define PATH_MAX 100
#define STATUS_SUCCESS 0
#define STATUS_FAIL -1

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ulimit.h>
#include <sys/resource.h>

extern char ** environ;

int printID();
int setProcessGroupLeader();
int printProcessIDs();
int printULimit();
int setUlimit();
int printCoreFileSize();
int setCoreFileSize();
int printCurDirectory();
int printEnvVariables();
int putEnv();


#endif //LAB1_OS_COMMANDS_H
