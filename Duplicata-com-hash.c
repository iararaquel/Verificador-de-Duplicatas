#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void freeTable(HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        Entry* current = ht->buckets[i];
        while (current) {
            Entry* temp = current;
            current = current->next;
            if (ht->freeKey) ht->freeKey(temp->key);
            if (ht->freeVal) ht->freeVal(temp->value);
            free(temp);
        }
    }
    free(ht->buckets);
    free(ht);
}

void printTable(HashTable* ht, PrintFunc printValue) {
    printf("\nTabela Hash (tamanho=%d, itens=%d):\n", ht->size, ht->count);
    for (int i = 0; i < ht->size; i++) {
        printf("[%d] -> ", i);
        Entry* current = ht->buckets[i];
        while (current) {
            printf("(");
            printValue(current->value);
            printf(") -> ");
            current = current->next;
        }
        printf("NULL\n");
    }
    printf("Colisões: %d | Load factor: %.2f\n", ht->collisions, (float)ht->count / ht->size);
}

// Função hash djb2 para strings
unsigned long hashString(void* key) {
    unsigned char* str = (unsigned char*)key;
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

// Comparação para strings
int cmpString(void* a, void* b) {
    return strcmp((char*)a, (char*)b);
}

// Libera string
void freeString(void* p) {
    free(p);
}

// Imprime inteiro
void printInt(void* val) {
    printf("%d", (int)val);
}

int main() {
    HashTable* ht = createHashTable(8, hashString, cmpString, freeString, free);

    char* keys[] = {"banana", "maçã", "laranja", "banana"};
    int values[] = {10, 20, 30, 40};

    for (int i = 0; i < 4; i++) {
        char* keyCopy = strdup(keys[i]);
        int* valPtr = malloc(sizeof(int));
        *valPtr = values[i];

        int inserted = insert(&ht, keyCopy, valPtr);
        if (inserted)
            printf("Inserido: %s -> %d\n", keyCopy, *valPtr);
        else {
            printf("Duplicata detectada para chave '%s', não inserido.\n", keyCopy);
            free(keyCopy);
            free(valPtr);
        }
    }

    printTable(ht, printInt);

    char* busca = "maçã";
    int* valor = get(ht, busca);
    if (valor)
        printf("\nValor para chave '%s': %d\n", busca, *valor);
    else
        printf("\nChave '%s' não encontrada\n", busca);

    printf("\nRemovendo chave 'banana'\n");
    if(removeKey(ht, "banana"))
        printf("Remoção realizada com sucesso.\n");
    else
        printf("Chave 'banana' não encontrada.\n");

    printTable(ht, printInt);

    freeTable(ht);
    return 0;
}

int main() {
    int numeros[] = {1, 2, 3, 4, 5, 3};  // exemplo
    int tamanho = sizeof(numeros) / sizeof(numeros[0]);

    clock_t inicio = clock(); 

    int resultado = temDuplicatas(numeros, tamanho);

    clock_t fim = clock(); 

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo: %f segundos\n", tempo);  

    if (resultado) {
        printf("Há duplicatas.\n");
    } else {
        printf("Não há duplicatas.\n");
    }

    return 0;
}