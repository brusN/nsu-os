#ifndef LAV4_OS_LIST_H
#define LAV4_OS_LIST_H

#include "stdio.h"
#include "malloc.h"
#include "string.h"

typedef struct st_String String;
struct st_String {
    char* string;
    int length;
};

typedef struct st_StringList StringList;
struct st_StringList {
    StringList *next;
    String value;
};

String *createNewString(char* string, int length);
StringList *createNode(char* string, int length);
StringList *addNode(StringList *head, StringList *node);
void printList(StringList *head);
void freeNode(StringList *node);
void freeList(StringList *head);

#endif //LAV4_OS_LIST_H
