#include <stdio.h>

void insertionSort(int arr[], int n) {
    int i, j, key;
// n = 5
    for (i = 1; i < n; i++) {
        key = arr[i];      // valor atual
        j = i - 1;

        // move os elementos maiores pra frente
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }

        arr[j + 1] = key;  // insere no lugar certo
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int arr[] = {5, 2, 12, 6, 1};
    int n = sizeof(arr) / sizeof(arr[0]); // saporra faz o calculo de saber o tamanho do array

    insertionSort(arr, n);
    printArray(arr, n);

    return 0;
}