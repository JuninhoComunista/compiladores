//Nome: Nicolas Paris - Cartao: 302650

#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

typedef struct HashNode {
    char* key;
    char* value;
    int type;
    int dataType;
} HashNode;

typedef struct LinkedList {
    HashNode* node;
    struct LinkedList* next;
} LinkedList;

typedef struct HashTable {
    HashNode **nodes;
    LinkedList** overflowBuckets;
    int size;
    int numElements;
} HashTable;

LinkedList* listAllocate();
LinkedList* listInsert(LinkedList* list, HashNode* node);
HashNode* listRemove(LinkedList* list);
void listDestroy(LinkedList* list);

unsigned long hashFunction(char *str);
HashNode* createNode(char *key, char *value, int type);
HashTable* createTable(int tableSize);
LinkedList** createOverflowBuckets(HashTable* table);
void destroyNode(HashNode* node);
void destroyTable(HashTable* table);
void destroyOverflowBuckets(HashTable* table);
HashNode* hashInsert(HashTable* table, char* key, char* value, int type);
HashNode* hashSearch(HashTable* table, char* key);
void handleCollision(HashTable* table, unsigned long index, HashNode* node);
void printTable(HashTable* table);