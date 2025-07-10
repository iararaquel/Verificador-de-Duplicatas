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