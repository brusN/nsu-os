#include <stdio.h>
#include "list.h"

#define BUFF_SIZE 1024

int main(void) {
    StringList *list = NULL;
    char *buffer = (char*)malloc(BUFF_SIZE + 1);
    int curBuffSize = 0;
    while (fgets(buffer, BUFF_SIZE, stdin)) {
        // Removing \n from input string
        curBuffSize = (int)strlen(buffer) - 1;
        buffer[curBuffSize] = '\0';

        list = addNode(list, createNode(buffer, curBuffSize));
        if (buffer[0] == '.')
            break;
    }
    printList(list);
    freeList(list);
    return 0;
}
