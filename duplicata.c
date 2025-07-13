#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long (*HashFunc)(void* key);
typedef int (*CmpFunc)(void* a, void* b);
typedef void (*FreeFunc)(void* data);
typedef void (*PrintFunc)(void* data);

typedef struct Entry {
    void* key;
    void* value;
    struct Entry* next;
} Entry;

typedef struct HashTable {
    Entry** buckets;
    int size;
    int count;
    int collisions;

    HashFunc hash;
    CmpFunc compare;
    FreeFunc freeKey;
    FreeFunc freeVal;
} HashTable;

#define LOAD_FACTOR_LIMIT 0.75

HashTable* createHashTable(int size, HashFunc hash, CmpFunc cmp, FreeFunc freeKey, FreeFunc freeVal) {
    HashTable* ht = malloc(sizeof(HashTable));
    ht->buckets = calloc(size, sizeof(Entry*));
    ht->size = size;
    ht->count = 0;
    ht->collisions = 0;
    ht->hash = hash;
    ht->compare = cmp;
    ht->freeKey = freeKey;
    ht->freeVal = freeVal;
    return ht;
}