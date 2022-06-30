#include "hash.h"

//Linked list functions

LinkedList* listAllocate() {
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    return list;
}

LinkedList* listInsert(LinkedList* list, HashNode* node) {
    if (!list) {
        LinkedList* head = listAllocate();
        head->node = node;
        head->next = NULL;
        list = head;
        return list;
    }
    
    LinkedList* temp = list;
    while (temp->next) {
        if (strcmp(temp->node->key, node->key) != 0) {
            LinkedList* newElement = listAllocate();
            newElement->node = node;
            newElement->next = list;
            return newElement;
        }
    }

    return list;
}

HashNode* listRemove(LinkedList* list) {
    if (!list || !list->next) {
        return NULL;
    }

    LinkedList* next = list->next;
    LinkedList* temp = list;
    temp->next = NULL;
    list = next;
    HashNode* it = NULL;
    memcpy(temp->node, it, sizeof(HashNode));
    free(temp->node->key);
    free(temp->node->value);
    free(temp->node);
    free(temp);
    return it;
}

void listDestroy(LinkedList* list) {
    LinkedList* temp = list;
    while (list)
    {
        temp = list;
        list = list->next;
        free(temp->node->key);
        free(temp->node->value);
        free(temp->node);
        free(temp);
    }
}

//HashTable functions

unsigned long hashFunction(char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c;

    return hash;
}

HashNode* createNode(char *key, char *value) {
    HashNode* node = (HashNode*) malloc(sizeof(HashNode));
    node->key = (char*) malloc(strlen(key)+1);
    node->value = (char*) malloc(strlen(value)+1);

    strcpy(node->key, key);
    strcpy(node->value, value);

    return node;
}

HashTable* createTable(int tableSize, float threshold) {
    HashTable* table = (HashTable*) malloc(sizeof(HashTable));
    table->size = tableSize;
    table->threshold = threshold;
    table->numElements = 0;
    table->nodes = (HashNode**) calloc(table->size, sizeof(HashNode*));
    table->overflowBuckets = createOverflowBuckets(table);
    for (int i=0; i < table->size; i++)
        table->nodes[i] = NULL;

    return table;
}

LinkedList** createOverflowBuckets(HashTable* table) {
    LinkedList** buckets = (LinkedList**) calloc(table->size, sizeof(LinkedList*));
    for (int i=0; i < table->size; i++)
        buckets[i] = NULL;
    return buckets;
}

void destroyNode(HashNode* node) {
    free(node->key);
    free(node->value);
    free(node);
}

void destroyTable(HashTable* table) {
    for (int i=0; i < table->size; i++) {
        HashNode* node = table->nodes[i];
        if(node != NULL) {
            destroyNode(node);
        }
    }
    destroyOverflowBuckets(table);
    free(table->nodes);
    free(table);
}

void destroyOverflowBuckets(HashTable* table) {
    LinkedList** buckets = table->overflowBuckets;
    for (int i=0; i < table->size; i++)
        listDestroy(buckets[i]);
    free(buckets);
}

void hashInsert(HashTable* table, char* key, char* value) {
    HashNode* node = createNode(key, value);
    unsigned long index;
    HashNode* currentNode;
    index = hashFunction(key)%table->size;
    currentNode = table->nodes[index];
    if (currentNode == NULL) {
        table->nodes[index] = node;
        table->numElements++;
    } else if (strcmp(currentNode->key, key) == 0) {
        strcpy(table->nodes[index]->value, value);
    } else {
        handleCollision(table, index, node);
        table->numElements++;
    }
    if (table->numElements >= (table->size * table->threshold)) {
        hashResize(table);
    }
}

char* hashSearch(HashTable* table, char* key) {
    unsigned long index = hashFunction(key);
    HashNode* node = table->nodes[index];
    LinkedList* head = table->overflowBuckets[index];

    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        if (head == NULL) {
            return NULL;
        }
        node = head->node;
        head = head->next;
    }

    return NULL;
}

void handleCollision(HashTable* table, unsigned long index, HashNode* node) {
    LinkedList* head = table->overflowBuckets[index];

    if (head == NULL) {
        head = listAllocate();
        head->node = node;
        table->overflowBuckets[index] = head;
    } else {
        table->overflowBuckets[index] = listInsert(head, node);
    }
}

HashTable* hashResize(HashTable* table) {
    int oldTableSize = table->size;
    float threshold = table->threshold;
    HashTable* oldTable = table;
    table = createTable(oldTableSize*2, threshold);
    for(int i=0; i < oldTableSize; i++) {
        if (oldTable->nodes[i]) {
            hashInsert(table, oldTable->nodes[i]->key, oldTable->nodes[i]->value);
            if (oldTable->overflowBuckets[i]) {
                LinkedList* head = oldTable->overflowBuckets[i];
                while(head) {
                    hashInsert(table, head->node->key, head->node->value);
                    head = head->next;
                }
            }
        }  
    }
    destroyTable(oldTable);
    return table;
}

void printTable(HashTable* table) {
    printf("\n-------------------\n");
    for (int i=0; i < table->size; i++) {
        if (table->nodes[i]) {
            printf("Index:%d, Key:%s, Value:%s", i, table->nodes[i]->key, table->nodes[i]->value);
            if (table->overflowBuckets[i]) {
                printf(" => Overflow Bucket => ");
                LinkedList* head = table->overflowBuckets[i];
                while (head) {
                    printf("Key:%s, Value:%s ", head->node->key, head->node->value);
                    head = head->next;
                }
            }
            printf("\n");
        }
    }
    printf("-------------------\n");
}