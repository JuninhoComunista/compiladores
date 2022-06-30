#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#ifndef HASH_H
#define HASH_H

typedef struct HashNode {
    char* key;
    char* value;
}HashNode;

typedef struct LinkedList LinkedList;
struct LinkedList {
    HashNode* node;
    LinkedList* next;
};

typedef struct HashTable {
    HashNode **nodes;
    LinkedList** overflowBuckets;
    int size;
    int numElements;
    float threshold;
} HashTable;

LinkedList* listAllocate();
LinkedList* listInsert(LinkedList* list, HashNode* node);
HashNode* listRemove(LinkedList* list);
void listDestroy(LinkedList* list);

unsigned long hashFunction(char *str);
HashNode* createNode(char *key, char *value);
HashTable* createTable(int tableSize, float threshold);
LinkedList** createOverflowBuckets(HashTable* table);
void destroyNode(HashNode* node);
void destroyTable(HashTable* table);
void destroyOverflowBuckets(HashTable* table);
void hashInsert(HashTable* table, char* key, char* value);
char* hashSearch(HashTable* table, char* key);
void handleCollision(HashTable* table, unsigned long index, HashNode* node);
HashTable* hashResize(HashTable* table);
void printTable(HashTable* table);

#endif

