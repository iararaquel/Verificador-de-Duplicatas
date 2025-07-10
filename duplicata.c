#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Fiz uma alteração aqui
typedef unsigned long (*HashFunc)(void* key);
typedef int (*CmpFunc)(void* a, void* b);
typedef void (*FreeFunc)(void* data);
typedef void (*PrintFunc)(void* data);