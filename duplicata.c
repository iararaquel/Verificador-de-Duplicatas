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

// Retorna 1 se inseriu, 0 se duplicata
int insert(HashTable** htRef, void* key, void* value) {
    HashTable* ht = *htRef;
    float load = (float)(ht->count + 1) / ht->size;
    if (load > LOAD_FACTOR_LIMIT) {
        rehash(htRef);
        ht = *htRef;
    }

    int index = ht->hash(key) % ht->size;
    Entry* current = ht->buckets[index];

    while (current) {
        if (ht->compare(current->key, key) == 0) {
            // Duplicata detectada
            return 0;
        }
        current = current->next;
    }

    if (ht->buckets[index] != NULL)
        ht->collisions++;

    Entry* newEntry = malloc(sizeof(Entry));
    newEntry->key = key;
    newEntry->value = value;
    newEntry->next = ht->buckets[index];
    ht->buckets[index] = newEntry;
    ht->count++;
    return 1;
}

void* get(HashTable* ht, void* key) {
    int index = ht->hash(key) % ht->size;
    Entry* current = ht->buckets[index];
    while (current) {
        if (ht->compare(current->key, key) == 0)
            return current->value;
        current = current->next;
    }
    return NULL;
}

// Retorna 1 se removeu, 0 se não achou
int removeKey(HashTable* ht, void* key) {
    int index = ht->hash(key) % ht->size;
    Entry* current = ht->buckets[index];
    Entry* prev = NULL;

    while (current) {
        if (ht->compare(current->key, key) == 0) {
            if (prev)
                prev->next = current->next;
            else
                ht->buckets[index] = current->next;

            if (ht->freeKey) ht->freeKey(current->key);
            if (ht->freeVal) ht->freeVal(current->value);
            free(current);
            ht->count--;
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}