#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ========================== ESTRUTURAS ==========================

typedef struct No {
    int chave;
    struct No *esq;
    struct No *dir;
} No;

// ========================== FUNÇÕES DA ÁRVORE ==========================

// Cria um novo nó
No* criarNo(int chave) {
    No *novo = (No *)malloc(sizeof(No));
    if (!novo) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }
    novo->chave = chave;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

// Inserção padrão em BST (sem balanceamento)
No* inserir(No *raiz, int chave) {
    if (raiz == NULL)
        return criarNo(chave);
    if (chave < raiz->chave)
        raiz->esq = inserir(raiz->esq, chave);
    else if (chave > raiz->chave)
        raiz->dir = inserir(raiz->dir, chave);
    // Ignora duplicatas
    return raiz;
}

// Constrói a melhor BST possível (sem rotações de balanceamento)
// Estratégia: insere a mediana primeiro, depois recursivamente
// as medianas das metades esquerda e direita
void construirMelhorBST(No **raiz, int arr[], int inicio, int fim) {
    if (inicio > fim)
        return;

    int meio = (inicio + fim) / 2;
    *raiz = inserir(*raiz, arr[meio]);

    construirMelhorBST(raiz, arr, inicio, meio - 1);
    construirMelhorBST(raiz, arr, meio + 1, fim);
}

// Percurso pré-ordem
void preOrdem(No *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->chave);
        preOrdem(raiz->esq);
        preOrdem(raiz->dir);
    }
}

// Calcula a altura da árvore
int altura(No *raiz) {
    if (raiz == NULL)
        return 0;
    int hEsq = altura(raiz->esq);
    int hDir = altura(raiz->dir);
    return 1 + (hEsq > hDir ? hEsq : hDir);
}

// Imprime a árvore de forma visual (rotacionada 90°)
void imprimirArvore(No *raiz, int nivel, char lado) {
    if (raiz == NULL)
        return;

    imprimirArvore(raiz->dir, nivel + 1, '/');

    for (int i = 0; i < nivel; i++)
        printf("        ");

    if (nivel == 0)
        printf("[%d]\n", raiz->chave);
    else
        printf("%c-- [%d]\n", lado, raiz->chave);

    imprimirArvore(raiz->esq, nivel + 1, '\\');
}

// Libera memória da árvore
void liberarArvore(No *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
    }
}

// ========================== FUNÇÕES DE ORDENAÇÃO E MERGE ==========================

// Ordenação por Insertion Sort (bom para arrays pequenos)
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int chave = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > chave) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = chave;
    }
}

// Merge de dois arrays ordenados
void merge(int a[], int na, int b[], int nb, int resultado[], int *nr) {
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb) {
        if (a[i] <= b[j])
            resultado[k++] = a[i++];
        else
            resultado[k++] = b[j++];
    }
    while (i < na) resultado[k++] = a[i++];
    while (j < nb) resultado[k++] = b[j++];
    *nr = k;
}

// Remove duplicatas de um array ordenado
int removerDuplicatas(int arr[], int n) {
    if (n <= 1) return n;
    int j = 0;
    for (int i = 1; i < n; i++) {
        if (arr[i] != arr[j])
            arr[++j] = arr[i];
    }
    return j + 1;
}

// ========================== VALIDAÇÃO ==========================

// Verifica se há elemento repetido no arranjo
int temRepetido(int arr[], int n) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] == arr[j])
                return 1;
    return 0;
}

// ========================== MAIN ==========================

int main() {
    int A, n;
    clock_t inicio_total, fim_total;
    clock_t inicio_etapa, fim_etapa;
    double tempo_ordenacao, tempo_merge, tempo_arvore, tempo_total;

    inicio_total = clock();

    // --- Entrada: quantidade de arranjos ---
    printf("============================================================\n");
    printf("   MELHOR ÁRVORE DE BUSCA A PARTIR DE ARRANJOS MESCLADOS\n");
    printf("============================================================\n\n");

    do {
        printf("Informe a quantidade de arranjos (1 a 10): ");
        scanf("%d", &A);
        if (A < 1 || A > 10)
            printf("Valor inválido! Deve ser entre 1 e 10.\n");
    } while (A < 1 || A > 10);

    // --- Entrada: tamanho dos arranjos ---
    do {
        printf("Informe a quantidade de elementos por arranjo (>= 1): ");
        scanf("%d", &n);
        if (n < 1)
            printf("Valor inválido! Deve ser >= 1.\n");
    } while (n < 1);

    // --- Alocação dos arranjos ---
    int **arranjos = (int **)malloc(A * sizeof(int *));
    for (int i = 0; i < A; i++)
        arranjos[i] = (int *)malloc(n * sizeof(int));

    // --- Leitura dos elementos ---
    for (int i = 0; i < A; i++) {
        printf("\n--- Arranjo %d (%d elementos, sem repetição) ---\n", i + 1, n);
        int valido;
        do {
            valido = 1;
            for (int j = 0; j < n; j++) {
                printf("  Elemento [%d]: ", j + 1);
                scanf("%d", &arranjos[i][j]);
            }
            if (temRepetido(arranjos[i], n)) {
                printf("  *** Erro: arranjo com elementos repetidos! Digite novamente. ***\n");
                valido = 0;
            }
        } while (!valido);
    }

    // ========================== ETAPA 1: ORDENAÇÃO ==========================
    printf("\n============================================================\n");
    printf("   ETAPA 1: ORDENAÇÃO DOS ARRANJOS\n");
    printf("============================================================\n");

    inicio_etapa = clock();

    for (int i = 0; i < A; i++)
        insertionSort(arranjos[i], n);

    fim_etapa = clock();
    tempo_ordenacao = ((double)(fim_etapa - inicio_etapa)) / CLOCKS_PER_SEC;

    for (int i = 0; i < A; i++) {
        printf("Arranjo %d ordenado: [ ", i + 1);
        for (int j = 0; j < n; j++)
            printf("%d ", arranjos[i][j]);
        printf("]\n");
    }
    printf("\nTempo de ordenação: %.6f segundos\n", tempo_ordenacao);

    // ========================== ETAPA 2: MERGE ==========================
    printf("\n============================================================\n");
    printf("   ETAPA 2: MERGE DE TODOS OS ARRANJOS\n");
    printf("============================================================\n");

    inicio_etapa = clock();

    int tam_total = A * n;
    int *merged = (int *)malloc(tam_total * sizeof(int));
    int *temp = (int *)malloc(tam_total * sizeof(int));

    // Copia o primeiro arranjo como base
    for (int j = 0; j < n; j++)
        merged[j] = arranjos[0][j];
    int tam_merged = n;

    // Merge iterativo com os demais arranjos
    for (int i = 1; i < A; i++) {
        int novo_tam;
        merge(merged, tam_merged, arranjos[i], n, temp, &novo_tam);
        // Copia resultado de volta para merged
        for (int j = 0; j < novo_tam; j++)
            merged[j] = temp[j];
        tam_merged = novo_tam;
    }

    // Remove duplicatas do array final
    tam_merged = removerDuplicatas(merged, tam_merged);

    fim_etapa = clock();
    tempo_merge = ((double)(fim_etapa - inicio_etapa)) / CLOCKS_PER_SEC;

    printf("Array mesclado (sem duplicatas): [ ");
    for (int i = 0; i < tam_merged; i++)
        printf("%d ", merged[i]);
    printf("]\n");
    printf("Total de elementos únicos: %d\n", tam_merged);
    printf("\nTempo de merge: %.6f segundos\n", tempo_merge);

    // ========================== ETAPA 3: CONSTRUÇÃO DA MELHOR BST ==========================
    printf("\n============================================================\n");
    printf("   ETAPA 3: CONSTRUÇÃO DA MELHOR BST (SEM BALANCEAMENTO)\n");
    printf("============================================================\n");

    inicio_etapa = clock();

    No *raiz = NULL;
    construirMelhorBST(&raiz, merged, 0, tam_merged - 1);

    fim_etapa = clock();
    tempo_arvore = ((double)(fim_etapa - inicio_etapa)) / CLOCKS_PER_SEC;

    printf("\nÁrvore construída com sucesso!\n");
    printf("Altura da árvore: %d\n", altura(raiz));
    printf("\nTempo de construção da árvore: %.6f segundos\n", tempo_arvore);

    // ========================== ETAPA 4: PRÉ-ORDEM ==========================
    printf("\n============================================================\n");
    printf("   ETAPA 4: PERCURSO EM PRÉ-ORDEM\n");
    printf("============================================================\n");

    printf("\nPré-ordem: ");
    preOrdem(raiz);
    printf("\n");

    // ========================== VISUALIZAÇÃO DA ÁRVORE ==========================
    printf("\n============================================================\n");
    printf("   VISUALIZAÇÃO DA ÁRVORE (rotacionada 90°)\n");
    printf("============================================================\n\n");

    imprimirArvore(raiz, 0, ' ');

    // ========================== RESUMO DOS TEMPOS ==========================
    fim_total = clock();
    tempo_total = ((double)(fim_total - inicio_total)) / CLOCKS_PER_SEC;

    printf("\n============================================================\n");
    printf("   RESUMO DOS TEMPOS DE EXECUÇÃO\n");
    printf("============================================================\n");
    printf("  Ordenação dos arranjos:       %.6f s\n", tempo_ordenacao);
    printf("  Merge dos arranjos:           %.6f s\n", tempo_merge);
    printf("  Construção da árvore BST:     %.6f s\n", tempo_arvore);
    printf("  Tempo total do programa:      %.6f s\n", tempo_total);
    printf("============================================================\n");

    // ========================== LIMPEZA ==========================
    liberarArvore(raiz);
    free(merged);
    free(temp);
    for (int i = 0; i < A; i++)
        free(arranjos[i]);
    free(arranjos);

    return 0;
}