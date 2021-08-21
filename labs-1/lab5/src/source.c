#include "source.h"

int safeOpen(const char *filePath, int mode) {
    int fileDescriptor = open(filePath, mode);
    if (fileDescriptor == -1) {
        perror("Can't open the input file: ");
        exit(1);
    }
    return fileDescriptor;
}

off_t *getOffSetTable(int offsetTableLength, int fd, int *totalLines) {
    off_t *offsetTable = (off_t *) malloc(offsetTableLength * sizeof(off_t));
    offsetTable[0] = 0;
    offsetTable[1] = 0;
    for (int i = 2; i < offsetTableLength; ++i) {
        offsetTable[i] = -1;
    }
    long curBuffLength = 0;
    int curLineLength = 0;
    int curLineNumber = 2;
    char *buffer = (char *)malloc(BUFF_SIZE);
    curBuffLength = read(fd, buffer, BUFF_SIZE);
    while (curBuffLength) {
        for (int i = 0; i < curBuffLength; ++i) {
            ++curLineLength;
            if (buffer[i] == '\n') {
                offsetTable[curLineNumber] = lseek(fd, offsetTable[curLineNumber - 1] + curLineLength, SEEK_SET);
                ++curLineNumber;
                curLineLength = 0;
            }
        }
        curBuffLength = read(fd, buffer, BUFF_SIZE);
    }
    *totalLines = curLineNumber - 2;
    free(buffer);
    return offsetTable;
}

int getLineNumber(char *buffer, size_t buffSize) {
    if (getline(&buffer, &buffSize, stdin) == 0) {
        printf("Error while reading from stdin!\n");
        exit(1);
    }
    int buffLength = (int)strlen(buffer) - 1;
    if (buffLength <= 0)
        return -1;
    buffer[buffLength] = 0;
    for (int i = 0; i < buffLength; ++i) {
        if (buffer[i] < '0' || buffer[i] > '9') {
            return -1;
        }
    }
    return (int)strtol(buffer, NULL, 10);
}

void printLine(int fd, const off_t *offsetTable, int offsetTableSize, char *buffer, int numLine) {
    long lineLength = offsetTable[numLine + 1] - offsetTable[numLine];
    lseek(fd, offsetTable[numLine], SEEK_SET);
    if (read(fd, buffer, lineLength)) {
        buffer[lineLength] = 0;
        printf("Line #%d: %s\n", numLine, buffer);
    } else {
        printf("Error while reading string!\n");
    }
}

void startLinePrinter(int fd, off_t *offsetTable, int offsetTableSize, int totalLines) {
    int curNumLine = -1;
    char *buffer = (char *)malloc(BUFF_SIZE);
    char *consoleBuffer = (char *)malloc(CONSOLE_BUFF_SIZE);
    printf("Total lines: %d\n", totalLines);
    do {
        printf("Enter line number: ");
        curNumLine = getLineNumber(consoleBuffer, CONSOLE_BUFF_SIZE);
        while (curNumLine < 0 || curNumLine > totalLines) {
            printf("Error while getting line number, try again: ");
            curNumLine = getLineNumber(consoleBuffer, BUFF_SIZE);
        }
        if (curNumLine == 0)
            return;
        printLine(fd, offsetTable, offsetTableSize, buffer, curNumLine);
    } while (curNumLine > 0);
    free(consoleBuffer);
    free(buffer);
}
