#include "list.h"

String *createNewString(char* string, int length) {
    String *newString = (String *)malloc(sizeof(String));
    memcpy(newString->string, string, length + 1);
    newString->length = length;
    return newString;
}

StringList *createNode(char* string, int length) {
    StringList *newNode = (StringList *)malloc(sizeof(String));
    newNode->value.string = (char*)malloc(length + 1);
    memcpy(newNode->value.string, string, length + 1);
    newNode->value.length = length;
    newNode->next = NULL;
    return newNode;
}

StringList *addNode(StringList *head, StringList *node) {
    if (head == NULL) {
        return node;
    }
    StringList *currentNode = head;
    while (currentNode->next) {
        currentNode = currentNode->next;
    }
    currentNode->next = node;
    return head;
}

void printList(StringList *head) {
    StringList *currentNode = head;
    while (currentNode) {
        printf("String: '%s', length: %d\n", currentNode->value.string, currentNode->value.length);
        currentNode = currentNode->next;
    }
}

void freeString(String *string) {
    free(string->string);
    free(string);
}

void freeNode(StringList *node) {
    free(node->value.string);
    free(node);
}

void freeList(StringList *head) {
    StringList *currentNode = head->next;
    while (currentNode) {
        StringList *temp = currentNode;
        currentNode = currentNode->next;
        freeNode(temp);
    }
    freeNode(head);
}


