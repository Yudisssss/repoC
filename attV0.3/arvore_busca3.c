#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct No {
    int chave;
    struct No *esq;
    struct No *dir;
} No;

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

void preOrdem(No *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->chave);
        preOrdem(raiz->esq);
        preOrdem(raiz->dir);
    }
}

void emOrdem(No *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq);
        printf("%d ", raiz->chave);
        emOrdem(raiz->dir);
    }
}

int altura(No *raiz) {
    if (raiz == NULL)
        return 0;
    int hEsq = altura(raiz->esq);
    int hDir = altura(raiz->dir);
    return 1 + (hEsq > hDir ? hEsq : hDir);
}

int contarNos(No *raiz) {
    if (raiz == NULL)
        return 0;
    return 1 + contarNos(raiz->esq) + contarNos(raiz->dir);
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

// ========================== GERAÇÃO ALEATÓRIA ==========================
void gerarValoresUnicos(int arr[], int n, int limite) {
    int *pool = (int *)malloc(limite * sizeof(int));
    for (int i = 0; i < limite; i++)
        pool[i] = i + 1;

    for (int i = 0; i < n; i++) {
        int j = i + rand() % (limite - i);
        int tmp = pool[i];
        pool[i] = pool[j];
        pool[j] = tmp;
        arr[i] = pool[i];
    }

    free(pool);
}

int main() {
    int A, n;
    clock_t inicio_total, fim_total;
    clock_t inicio_etapa, fim_etapa;
    double tempo_merge, tempo_arvore, tempo_total;

    srand((unsigned)time(NULL));

    inicio_total = clock();

    printf("============================================================\n");
    printf("   ÁRVORE DE BUSCA A PARTIR DAS ARRAYS\n");
    printf("============================================================\n\n");

    do {
        printf("Informe a quantidade de arranjos (1 a 10): ");
        scanf("%d", &A);
        if (A < 1 || A > 10)
            printf("Valor inválido! Deve ser entre 1 e 10.\n");
    } while (A < 1 || A > 10);

    do {
        printf("Informe a quantidade de elementos por arranjo (>= 1): ");
        scanf("%d", &n);
        if (n < 1)
            printf("Valor inválido! Deve ser >= 1.\n");
    } while (n < 1);

    int limite = A * n * 3;
    if (limite < 100) limite = 100;

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

    printf("\n============================================================\n");
    printf("   ETAPA 1: MERGE DOS ARRAYS \n");
    printf("============================================================\n");

    inicio_etapa = clock();

    int tam_total = A * n;
    int *merged = (int *)malloc(tam_total * sizeof(int));

    int pos = 0;
    for (int i = 0; i < A; i++)
        for (int j = 0; j < n; j++)
            merged[pos++] = arranjos[i][j];

    fim_etapa = clock();
    tempo_merge = ((double)(fim_etapa - inicio_etapa)) / CLOCKS_PER_SEC;

    printf("Array mesclado (ordem original): [ ");
    for (int i = 0; i < tam_total; i++)
        printf("%d ", merged[i]);
    printf("]\n");
    printf("Total de elementos: %d\n", tam_total);
    printf("\nTempo de merge: %.6f segundos\n", tempo_merge);

    printf("\n============================================================\n");
    printf("   ETAPA 2: CONSTRUÇÃO DA ARVORE (SEM BALANCEAMENTO)\n");
    printf("============================================================\n");

    inicio_etapa = clock();

    No *raiz = NULL;

    for (int i = 0; i < tam_total; i++)
        raiz = inserir(raiz, merged[i]);

    fim_etapa = clock();
    tempo_arvore = ((double)(fim_etapa - inicio_etapa)) / CLOCKS_PER_SEC;

    int totalNos = contarNos(raiz);
    printf("\nÁrvore construída com sucesso!\n");
    printf("Nós inseridos (sem duplicatas): %d\n", totalNos);
    printf("Altura da árvore: %d\n", altura(raiz));
    printf("\nTempo de construção da árvore: %.6f segundos\n", tempo_arvore);

    printf("\n============================================================\n");
    printf("   ETAPA 3: PERCURSO EM PRÉ-ORDEM\n");
    printf("============================================================\n");

    printf("\nPré-ordem: ");
    preOrdem(raiz);
    printf("\n");

    printf("\n============================================================\n");
    printf("   PROVA: PERCURSO EM-ORDEM (elementos ordenados pela ARVORE)\n");
    printf("============================================================\n");

    printf("\nEm-ordem: ");
    emOrdem(raiz);
    printf("\n");

    printf("\n============================================================\n");
    printf("   VISUALIZAÇÃO DA ARVORE\n");
    printf("============================================================\n\n");

    imprimirArvore(raiz, 0, ' ');
    fim_total = clock();
    tempo_total = ((double)(fim_total - inicio_total)) / CLOCKS_PER_SEC;

    printf("\n============================================================\n");
    printf("   RESUMO DOS TEMPOS DE EXECUÇÃO\n");
    printf("============================================================\n");
    printf("  Merge (concatenação):         %.6f s\n", tempo_merge);
    printf("  Construção da árvore BST:     %.6f s\n", tempo_arvore);
    printf("  Tempo total do programa:      %.6f s\n", tempo_total);
    printf("============================================================\n");

    liberarArvore(raiz);
    free(merged);
    for (int i = 0; i < A; i++)
        free(arranjos[i]);
    free(arranjos);

    return 0;
}