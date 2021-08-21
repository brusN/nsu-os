#include "source.h"

int executeCommand(const int optionIndex) {
    switch (optionIndex) {
        case 'i': return printID();
        case 's': return setProcessGroupLeader();
        case 'p': return printProcessIDs();
        case 'u': return printULimit();
        case 'U': return setUlimit();
        case 'c': return printCoreFileSize();
        case 'C': return setCoreFileSize();
        case 'd': return printCurDirectory();
        case 'v': return printEnvVariables();
        case 'V': return putEnv();
        case '?': {
            printf("Command hasn't recognized!");
            return STATUS_SUCCESS;
        }
        default: {
            printf("Unknown command! Execution has canceled!");
            exit(1);
        }
    }
}

void printUsage() {
    printf("Usage:\n"
           "    -i <Prints real and effective user and group IDs>\n"
           "    -s <The process becomes the leader of the group>\n"
           "    -p <Prints the IDs of the process, parent process, and process group>\n"
           "    -u <Prints the ulimit value>\n"
           "    -Unew_ulimit <Modifies the ulimit value>\n"
           "    -c <Prints the size in bytes of the core file that can be created>\n"
           "    -Csize <Resizes the core file>\n"
           "    -d <Prints the current working directory>\n"
           "    -v <Prints out environment variables and their values>\n"
           "    -Vname=value <Introduces a new variable to the environment or changes the value of an existing variable>\n");
}
