#include <string.h>
#define INITIAL_SIZE 997
int table_size = INITIAL_SIZE;

typedef struct HashNode {
    char* key;
    char* value;
}HashNode;

typedef struct HashTable {
    HashNode **nodes;
    int size;
    int numElements;
} HashTable;

unsigned long hash_function(char *str);
HashNode* createNode(char *key, char *value);
HashTable* createTable(int size);
void destroyNode(HashNode* node);
void destroyTable(HashTable* table)

unsigned long hash_function(char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c;

    return hash%table_size;
}

HashNode* createNode(char *key, char *value) {
    HashNode* node = (HashNode*) malloc(sizeof(HashNode));
    node->key = (char*) malloc(strlen(key)+1);
    node->value = (char*) malloc(strlen(value)+1);

    strcpy(node->key, key);
    strcpy(node->value, value);

    return node;
}

HashTable* createTable(int size) {
    HashTable* table = (HashTable*) malloc(sizeof(HashTable));
    table->size = size;
    table->numElements = 0;
    table->nodes = (HashNode**) calloc(table->size, sizeof(HashNode*));
    for (int i=0; i < table->size; i++)
        table->nodes[i] = NULL;

    return table;
}

void destroyNode(HashNode* node) {
    free(node->key);
    free(node->value);
    free(node);
}

void destroyTable(HashTable* table) {

}

