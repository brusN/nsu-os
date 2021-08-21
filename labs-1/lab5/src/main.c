#include <stdio.h>
#include "source.h"

int main(int argc, char **argv) {
    int fd = safeOpen(argv[1], O_RDONLY);
    int offsetTableSize = OFFSET_TABLE_SIZE_DEFAULT;
    int totalLines = 0;
    off_t *offsetTable = getOffSetTable(offsetTableSize, fd, &totalLines);
    startLinePrinter(fd, offsetTable, offsetTableSize, totalLines);
    free(offsetTable);
    close(fd);
    return 0;
}
