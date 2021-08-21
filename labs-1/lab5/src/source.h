#ifndef LAB5_V2_OS_SOURCE_H
#define LAB5_V2_OS_SOURCE_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "string.h"

#define CONSOLE_BUFF_SIZE 32
#define BUFF_SIZE 512
#define OFFSET_TABLE_SIZE_DEFAULT 100

int safeOpen(const char *filePath, int mode);
off_t *getOffSetTable(int offsetTableLength, int fd, int *totalLines);
int getLineNumber(char *buffer, size_t buffSize);
void printLine(int fd, const off_t *offsetTable, int offsetTableSize, char *buffer, int numLine);
void startLinePrinter(int fd, off_t *offsetTable, int offsetTableSize, int totalLines);

#endif //LAB5_V2_OS_SOURCE_H
