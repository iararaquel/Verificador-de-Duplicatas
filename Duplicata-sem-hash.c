#include <stdio.h>

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