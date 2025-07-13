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

static void rehash(HashTable** htRef) {
    HashTable* old = *htRef;
    int newSize = old->size * 2;
    HashTable* newTable = createHashTable(newSize, old->hash, old->compare, old->freeKey, old->freeVal);

    for (int i = 0; i < old->size; i++) {
        Entry* current = old->buckets[i];
        while (current) {
            // Inserção sem duplicata garantida na tabela nova
            Entry* next = current->next;
            current->next = NULL; // desligar para evitar ligação errada
            // Inserir diretamente para preservar valor e chave
            int index = newTable->hash(current->key) % newTable->size;
            if (newTable->buckets[index] != NULL)
                newTable->collisions++;
            current->next = newTable->buckets[index];
            newTable->buckets[index] = current;
            newTable->count++;
            current = next;
        }
    }
    free(old->buckets);
    free(old);
    *htRef = newTable;
}