#include <stdio.h>

// Função para imprimir o array
void imprimirArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Função do Insertion Sort
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int chave = arr[i];   // elemento atual que queremos posicionar
        int j = i - 1;

        // Move os elementos maiores que a chave
        // uma posição à frente
        while (j >= 0 && arr[j] > chave) {
            arr[j + 1] = arr[j];
            j--;
        }

        // Coloca a chave na posição correta
        arr[j + 1] = chave;
    }
}

int main() {
    int arr[] = {5, 3, 4, 1, 2};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array original:\n");
    imprimirArray(arr, n);

    insertionSort(arr, n);

    printf("Array ordenado:\n");
    imprimirArray(arr, n);

    return 0;
}