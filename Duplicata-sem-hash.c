#include <stdio.h>
#include <time.h>

#define MAX 100

// Função que verifica duplicatas em um array de inteiros
int temDuplicatas(int array[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = i + 1; j < tamanho; j++) {
            if (array[i] == array[j]) {
                return 1; // Encontrou duplicata
            }
        }
    }
    return 0; // Nenhuma duplicata
}

int main() {
    int numeros[MAX];
    int n;

    printf("Quantos números deseja inserir (máx %d)? ", MAX);
    scanf("%d", &n);

    if (n > MAX || n <= 0) {
        printf("Quantidade inválida.\n");
        return 1;
    }

    printf("Digite %d números:\n", n);
    for (int i = 0; i < n; i++) {
        printf("Número %d: ", i + 1);
        scanf("%d", &numeros[i]);
    }

    if (temDuplicatas(numeros, n)) {
        printf("\nHá duplicatas na lista.\n");
    } else {
        printf("\nNão há duplicatas na lista.\n");
    }

    return 0;
}

int main() {
    int numeros[] = {1, 2, 3, 4, 5, 3};  // exemplo
    int tamanho = sizeof(numeros) / sizeof(numeros[0]);

    clock_t inicio = clock(); // ⏱️ começa medição

    int resultado = temDuplicatas(numeros, tamanho);

    clock_t fim = clock(); // ⏱️ termina medição

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo: %f segundos\n", tempo);  // mostra o tempo

    if (resultado) {
        printf("Há duplicatas.\n");
    } else {
        printf("Não há duplicatas.\n");
    }

    return 0;
}