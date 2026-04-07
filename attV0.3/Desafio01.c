/*DESAFIO: Dividir um arranjo em 3 partes jogando uma para
 *selection outro para inserction e a ultima para burble sort.
 *ao retornar os arrays ordenados faca um merge*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 30

void selection_sort(int v[], int n) {
    int i, j, min, tmp;
    for (i = 0; i < n - 1; i++) {
        min = i;
        for (j = i + 1; j < n; j++)
            if (v[j] < v[min]) min = j;
        tmp = v[i]; v[i] = v[min]; v[min] = tmp;
    }
}

void insertion_sort(int v[], int n) {
    int i, j, chave;
    for (i = 1; i < n; i++) {
        chave = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > chave) { v[j + 1] = v[j]; j--; }
        v[j + 1] = chave;
    }
}

void bubble_sort(int v[], int n) {
    int i, j, tmp;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (v[j] > v[j + 1]) {
                tmp = v[j]; v[j] = v[j + 1]; v[j + 1] = tmp;
            }
}

void merge(int v[], int ini, int meio, int fim) {
    int i, j, k;
    int n1 = meio - ini + 1, n2 = fim - meio;
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++) L[i] = v[ini + i];
    for (j = 0; j < n2; j++) R[j] = v[meio + 1 + j];

    i = 0; j = 0; k = ini;
    while (i < n1 && j < n2)
        v[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) v[k++] = L[i++];
    while (j < n2) v[k++] = R[j++];

    free(L); free(R);
}

void imprimir(int v[], int n, const char *label) {
    int i;
    printf("\n%s (%d elementos):\n", label, n);
    for (i = 0; i < n; i++) printf("%3d", v[i]);
    printf("\n");
}

int main(void) {
    int arr[TAM];
    int i, p1, p2;

    srand(time(NULL));

    for (i = 0; i < TAM; i++)
        arr[i] = rand() % 99 + 1;

    imprimir(arr, TAM, "Original");

    p1 = TAM / 3;
    p2 = 2 * (TAM / 3);

    /* divisão antes de ordenar */
    printf("\n====== DIVISAO ======\n");
    imprimir(arr,      p1,        "Parte 1 (selection)");
    imprimir(arr + p1, p2 - p1,  "Parte 2 (insertion)");
    imprimir(arr + p2, TAM - p2, "Parte 3 (bubble)   ");

    selection_sort(arr,       p1);
    insertion_sort(arr + p1,  p2 - p1);
    bubble_sort   (arr + p2,  TAM - p2);

    merge(arr, 0,    p1 - 1, p2 - 1);
    merge(arr, 0,    p2 - 1, TAM - 1);

    printf("\n====== RESULTADO FINAL ======\n");
    imprimir(arr, TAM, "Array completo ordenado");

    return 0;
}