#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_IMPRESSORAS 5
#define MAX_DOCUMENTOS 100

typedef enum {
    URGENTE = 0,
    NORMAL = 1
} Prioridade;

typedef struct {
    int id;
    char nome[50];
    int paginas;
    Prioridade prioridade;
} Documento;

typedef struct No {
    Documento documento;
    struct No *proximo;
} No;

typedef struct {
    No *inicio;
    No *fim;
    int tamanho;
} Fila;

//  FUNÇÕES DA FILA

void initFila(Fila *fila){
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
}

void enqueue_normal(Fila *fila, Documento doc) {
    No *novo = (No*) malloc(sizeof(No));
    novo->documento = doc;
    novo->proximo = NULL;

    if (fila->inicio == NULL){
        fila->inicio = novo;
        fila->fim = novo;
    } else {
        fila->fim->proximo = novo;
        fila->fim = novo;
    }

    fila->tamanho++;
}

void enqueue_urgente(Fila *fila, Documento doc) {
    No *novo = (No*) malloc(sizeof(No));
    novo->documento = doc;
    novo->proximo = fila->inicio;
    fila->inicio = novo;

    if (fila->fim == NULL)
        fila->fim = novo;

    fila->tamanho++;
}

Documento dequeue(Fila *fila){
    Documento vazio = {0};

    if (fila->inicio == NULL){
        printf("Fila vazia!\n");
        return vazio;
    }

    No *temp = fila->inicio;
    Documento valor = temp->documento;

    fila->inicio = temp->proximo;

    if (fila->inicio == NULL)
        fila->fim = NULL;

    free(temp);
    fila->tamanho--;

    return valor;
}

//  LISTAR FILA

void listarFila(Fila *fila) {
    if (fila->inicio == NULL) {
        printf("\nFila vazia.\n");
        return;
    }

    No *atual = fila->inicio;

    printf("\n--- Fila de Impressão ---\n");
    while (atual != NULL) {
        printf("ID: %d | Nome: %s | Prioridade: %s\n",
               atual->documento.id,
               atual->documento.nome,
               atual->documento.prioridade == URGENTE ? "URGENTE" : "NORMAL");
        atual = atual->proximo;
    }
}

//  MENU

int main() {
    Fila fila;
    initFila(&fila);

    int opcao;

    do {
        printf("\n======================================\n");
        printf("=== Simulador de Impressoras em Rede ===\n");
        printf("======================================\n");
        printf("1 - Adicionar Impressão\n");
        printf("2 - Listar Fila\n");
        printf("3 - Imprimir Documento\n");
        printf("0 - Sair\n");
        printf("> ");
        scanf("%d", &opcao);

        switch (opcao) {

        case 1: {
            Documento doc;
            char nome[50];

            printf("Nome do documento: ");
            scanf("%s", nome);

            strcpy(doc.nome, nome);
            doc.id = fila.tamanho + 1;
            doc.paginas = 1;

            int p;
            printf("Prioridade: 1-Urgente  2-Normal: ");
            scanf("%d", &p);

            doc.prioridade = (p == 1 ? URGENTE : NORMAL);

            if (doc.prioridade == URGENTE)
                enqueue_urgente(&fila, doc);
            else
                enqueue_normal(&fila, doc);

            break;
        }

        case 2:
            listarFila(&fila);
            break;

        case 3: {
            Documento d = dequeue(&fila);
            if (d.id != 0)
                printf("Imprimindo: %s\n", d.nome);
            break;
        }

        }

    } while (opcao != 0);

    return 0;
}
