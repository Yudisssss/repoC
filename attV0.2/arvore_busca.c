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

No* inserir(No *raiz, int chave) {
    if (raiz == NULL)
        return criarNo(chave);
    if (chave < raiz->chave)
        raiz->esq = inserir(raiz->esq, chave);
    else if (chave > raiz->chave)
        raiz->dir = inserir(raiz->dir, chave);
    return raiz;
}

void construirMelhorBST(No **raiz, int arr[], int inicio, int fim) {
    if (inicio > fim)
        return;
    int meio = (inicio + fim) / 2;
    *raiz = inserir(*raiz, arr[meio]);
    construirMelhorBST(raiz, arr, inicio, meio - 1);
    construirMelhorBST(raiz, arr, meio + 1, fim);
}

void preOrdem(No *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->chave);
        preOrdem(raiz->esq);
        preOrdem(raiz->dir);
    }
}

int altura(No *raiz) {
    if (raiz == NULL)
        return 0;
    int hEsq = altura(raiz->esq);
    int hDir = altura(raiz->dir);
    return 1 + (hEsq > hDir ? hEsq : hDir);
}

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

void liberarArvore(No *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
    }
}

// ========================== ORDENAÇÃO E MERGE ==========================

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

int removerDuplicatas(int arr[], int n) {
    if (n <= 1) return n;
    int j = 0;
    for (int i = 1; i < n; i++) {
        if (arr[i] != arr[j])
            arr[++j] = arr[i];
    }
    return j + 1;
}

// ========================== GERAÇÃO ALEATÓRIA ==========================

// Gera n valores únicos aleatórios no intervalo [1, limite]
// Usa embaralhamento parcial (Fisher-Yates) para garantir unicidade
void gerarValoresUnicos(int arr[], int n, int limite) {
    int *pool = (int *)malloc(limite * sizeof(int));
    for (int i = 0; i < limite; i++)
        pool[i] = i + 1;

    // Fisher-Yates shuffle parcial
    for (int i = 0; i < n; i++) {
        int j = i + rand() % (limite - i);
        int tmp = pool[i];
        pool[i] = pool[j];
        pool[j] = tmp;
        arr[i] = pool[i];
    }

    free(pool);
}

// ========================== MAIN ==========================

int main() {
    int A, n;
    clock_t inicio_total, fim_total;
    clock_t inicio_etapa, fim_etapa;
    double tempo_ordenacao, tempo_merge, tempo_arvore, tempo_total;

    srand((unsigned)time(NULL));

    inicio_total = clock();

    printf("============================================================\n");
    printf("   MELHOR ÁRVORE DE BUSCA A PARTIR DE ARRANJOS MESCLADOS\n");
    printf("============================================================\n\n");

    // --- Entrada: quantidade de arranjos ---
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

    // Limite superior dos valores: 3x o total para variedade
    int limite = A * n * 3;
    if (limite < 100) limite = 100;

    // --- Alocação e geração automática ---
    int **arranjos = (int **)malloc(A * sizeof(int *));
    for (int i = 0; i < A; i++)
        arranjos[i] = (int *)malloc(n * sizeof(int));

    printf("\n--- Valores gerados automaticamente (intervalo 1 a %d) ---\n", limite);
    for (int i = 0; i < A; i++) {
        gerarValoresUnicos(arranjos[i], n, limite);
        printf("Arranjo %d: [ ", i + 1);
        for (int j = 0; j < n; j++)
            printf("%d ", arranjos[i][j]);
        printf("]\n");
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

    for (int j = 0; j < n; j++)
        merged[j] = arranjos[0][j];
    int tam_merged = n;

    for (int i = 1; i < A; i++) {
        int novo_tam;
        merge(merged, tam_merged, arranjos[i], n, temp, &novo_tam);
        for (int j = 0; j < novo_tam; j++)
            merged[j] = temp[j];
        tam_merged = novo_tam;
    }

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
