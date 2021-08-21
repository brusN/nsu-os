#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define STATUS_SUCCESS 0
#define STATUS_FAIl -1

int main(int argc, char **argv) {
    FILE* file = fopen(argv[1], "r");
    printf("Real UID: %d\nEffective UID: %d\n", getuid(), geteuid());

    if (file == NULL) {
        perror("File hasn't opened");
    } else {
        printf("File has opened!\n");
        if (fclose(file) != STATUS_SUCCESS) {
            perror("Error closing file!");
            exit(STATUS_FAIl);
        }
    }

    if (setuid(getuid()) == STATUS_FAIl) {
        perror("UID hasn't set!");
        exit(STATUS_FAIl);
    }

    printf("New real UID: %d\nNew effective UID: %d\n", getuid(), geteuid());

    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("File hasn't opened");
        exit(STATUS_FAIl);
    }
    printf("File has opened!\n");

    if (fclose(file) != STATUS_SUCCESS) {
        perror("Error closing file!");
        exit(STATUS_FAIl);
    }

    return 0;
}