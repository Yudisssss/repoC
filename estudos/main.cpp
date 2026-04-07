#include <stdlib.h>
#include <stdio.h>

enum cor = {VERMELHO, PRETO};

typedef struct no *p_no;

struct no {
    int valor;
    enum cor cor;
    p_no esquerda, direita;
}

int ehVermelho(n_no, x) {
    if (x == NULL) return 0;
    return x->cor == vermelho;
}

int ehPreto(n_no, x) {
    if (x == NULL) return 1;
    return x->cor == preto;
}

int main() {
    p_no raiz = NULL;

}