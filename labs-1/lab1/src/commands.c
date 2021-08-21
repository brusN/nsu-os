#include <stdlib.h>
#include "commands.h"

int printID() {
    printf("RID: %d\n"
           "EUID: %d\n"
           "RGID: %d\n"
           "EGID: %d\n",
          getuid(), geteuid(), getgid(), getegid());
    return STATUS_SUCCESS;
}

int setProcessGroupLeader() {
    if (setpgid(0, 0) != 0) {
        perror("Error while process group leader setting!");
        return STATUS_FAIL;
    }
    return STATUS_SUCCESS;
}

int printProcessIDs() {
    printf("PID: %d\n"
           "PPID: %d\n"
           "GPID: %d\n",
           getpid(), getppid(), getpgid(0));
    return STATUS_SUCCESS;
}

int printULimit() {
    long ulimitSize = ulimit(UL_GETFSIZE);
    if (ulimitSize == -1) {
        perror("Error while getting ulimit size!");
        return STATUS_FAIL;
    }
    printf("ulimit value: %ld\n", ulimitSize);
    return STATUS_SUCCESS;
}

int setUlimit() {
    long newValue = strtol(optarg, NULL, 10);
    if (newValue < 0) {
        perror("Error while converting new ulimit value!");
        return STATUS_FAIL;
    }

    if (ulimit(UL_SETFSIZE, newValue) == -1) {
        perror("Error while setting ulimit value!");
        return STATUS_FAIL;
    }
    return STATUS_SUCCESS;
}

int printCoreFileSize() {
    struct rlimit rlp;
    if (getrlimit(RLIMIT_CORE, &rlp) == -1) {
        perror("Error while getting core file size!");
        return STATUS_FAIL;
    }
    printf("Current core file size: %ld\n", rlp.rlim_cur);
    return STATUS_SUCCESS;
}

int setCoreFileSize() {
    struct rlimit rlp;
    if (getrlimit(RLIMIT_CORE, &rlp) == -1) {
        perror("Error while getting core file size!");
        return STATUS_FAIL;
    }

    long newCoreFileSize = strtol(optarg, NULL, 10);
    if (newCoreFileSize < 0) {
        perror("Error while converting new core file size!");
        return STATUS_FAIL;
    }

    rlp.rlim_cur = newCoreFileSize;
    if (setrlimit(RLIMIT_CORE, &rlp) == -1) {
        perror("Error while setting new core file size!");
        return STATUS_FAIL;
    }

    return STATUS_SUCCESS;
}

int printCurDirectory() {
    char* directoryPath = getcwd(NULL, PATH_MAX);
    if (directoryPath == NULL) {
        perror("Error while getting current working directory");
        return STATUS_FAIL;
    }

    printf("Current working directory: %s\n", directoryPath);
    return STATUS_SUCCESS;
}

int printEnvVariables() {
    printf("Environment variables list: \n");
    for (char** envVar = environ; *envVar; ++envVar)
        printf("%s\n", *envVar);

    return STATUS_SUCCESS;
}

int putEnv() {
    if (putenv(optarg) != 0) {
        perror("There are problems with putting new environmental variable");
        return STATUS_FAIL;
    }
    return STATUS_SUCCESS;
}












