#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes
#define MAX_DOCUMENTOS 100
#define MAX_IMPRESSORAS 5
#define MAX_NOME 50

// Enumeração de prioridades
typedef enum {
    BAIXA = 0,
    NORMAL = 1,
    URGENTE = 2
} Prioridade;

// Estrutura do documento
typedef struct {
    int id;
    char nome[MAX_NOME];
    Prioridade prioridade;
    int paginas;
} Documento;

// Estrutura do nó da fila
typedef struct Node {
    Documento documento;
    struct Node* next;
} Node;

// Estrutura da fila de prioridade
typedef struct {
    Node* front;
    Node* rear;
    int tamanho;
} FilaPrioridade;

// Estrutura da impressora
typedef struct {
    int id;
    int disponivel; // 1 = disponível, 0 = ocupada
    Documento documento_atual;
    int documentos_impressos;
} Impressora;

// Funções da fila de prioridade
void inicializar_fila(FilaPrioridade* fila) {
    fila->front = NULL;
    fila->rear = NULL;
    fila->tamanho = 0;
}

// Função para obter o valor numérico da prioridade
int valor_prioridade(Prioridade p) {
    return (int)p;
}

// Inserir documento na fila com base na prioridade
void inserir_na_fila(FilaPrioridade* fila, Documento doc) {
    Node* novo_no = (Node*)malloc(sizeof(Node));
    novo_no->documento = doc;
    novo_no->next = NULL;
    
    // Se a fila estiver vazia ou o novo documento tiver prioridade maior que o primeiro
    if (fila->front == NULL || 
        valor_prioridade(doc.prioridade) > valor_prioridade(fila->front->documento.prioridade)) {
        novo_no->next = fila->front;
        fila->front = novo_no;
        
        if (fila->rear == NULL) {
            fila->rear = novo_no;
        }
    } else {
        // Encontrar a posição correta baseada na prioridade
        Node* atual = fila->front;
        Node* anterior = NULL;
        
        while (atual != NULL && 
               valor_prioridade(doc.prioridade) <= valor_prioridade(atual->documento.prioridade)) {
            anterior = atual;
            atual = atual->next;
        }
        
        if (anterior == NULL) {
            novo_no->next = fila->front;
            fila->front = novo_no;
        } else {
            anterior->next = novo_no;
            novo_no->next = atual;
            
            if (atual == NULL) {
                fila->rear = novo_no;
            }
        }
    }
    
    fila->tamanho++;
}

// Remover documento da fila (sempre o de maior prioridade)
Documento remover_da_fila(FilaPrioridade* fila) {
    Documento doc_vazio = {0};
    
    if (fila->front == NULL) {
        return doc_vazio;
    }
    
    Node* temp = fila->front;
    Documento doc = temp->documento;
    fila->front = fila->front->next;
    
    if (fila->front == NULL) {
        fila->rear = NULL;
    }
    
    free(temp);
    fila->tamanho--;
    return doc;
}

// Exibir conteúdo da fila
void exibir_fila(FilaPrioridade* fila) {
    printf("\n=== FILA DE DOCUMENTOS ===\n");
    printf("Documentos na fila: %d\n\n", fila->tamanho);
    
    if (fila->front == NULL) {
        printf("Fila vazia!\n");
    } else {
        Node* atual = fila->front;
        int posicao = 1;
        
        while (atual != NULL) {
            char prioridade_str[20];
            switch (atual->documento.prioridade) {
                case URGENTE: strcpy(prioridade_str, "URGENTE"); break;
                case NORMAL: strcpy(prioridade_str, "NORMAL"); break;
                case BAIXA: strcpy(prioridade_str, "BAIXA"); break;
            }
            
            printf("%d. [ID: %d] %s - %d paginas - Prioridade: %s\n", 
                   posicao++, 
                   atual->documento.id,
                   atual->documento.nome,
                   atual->documento.paginas,
                   prioridade_str);
            atual = atual->next;
        }
    }
    printf("===========================\n");
}

// Função para adicionar documentos
void adicionar_documento(FilaPrioridade* fila, int* documento_counter) {
    Documento doc;
    (*documento_counter)++;
    
    doc.id = *documento_counter;
    
    printf("\n=== ADICIONAR DOCUMENTO ===\n");
    printf("Nome do documento: ");
    scanf(" %[^\n]", doc.nome);
    
    printf("Prioridade (0=Baixa, 1=Normal, 2=Urgente): ");
    int prioridade;
    scanf("%d", &prioridade);
    
    if (prioridade < 0 || prioridade > 2) prioridade = 1;
    doc.prioridade = (Prioridade)prioridade;
    
    printf("Numero de paginas: ");
    scanf("%d", &doc.paginas);
    
    if (doc.paginas < 1) doc.paginas = 1;
    if (doc.paginas > 50) doc.paginas = 50;
    
    inserir_na_fila(fila, doc);
    
    printf("Documento '%s' adicionado a fila com ID %d\n", doc.nome, doc.id);
}

// Função para processar documentos (simulação simplificada)
void processar_documentos(FilaPrioridade* fila, Impressora impressoras[], int num_impressoras) {
    printf("\n=== PROCESSANDO DOCUMENTOS ===\n");
    
    for (int i = 0; i < num_impressoras; i++) {
        if (impressoras[i].disponivel && fila->front != NULL) {
            Documento doc = remover_da_fila(fila);
            
            if (doc.id != 0) {
                impressoras[i].disponivel = 0;
                impressoras[i].documento_atual = doc;
                
                char prioridade_str[20];
                switch (doc.prioridade) {
                    case URGENTE: strcpy(prioridade_str, "URGENTE"); break;
                    case NORMAL: strcpy(prioridade_str, "NORMAL"); break;
                    case BAIXA: strcpy(prioridade_str, "BAIXA"); break;
                }
                
                printf("[Impressora %d] Imprimindo: %s (ID: %d, %d paginas, %s)\n", 
                       impressoras[i].id, doc.nome, doc.id, doc.paginas, prioridade_str);
                
                // Simulação simples: cada página leva 1 "ciclo"
                printf("Progresso: [");
                for (int p = 0; p < doc.paginas; p++) {
                    printf("#");
                }
                printf("] - %d paginas\n", doc.paginas);
                
                impressoras[i].documentos_impressos++;
                impressoras[i].disponivel = 1;
                printf("[Impressora %d] Concluido: %s\n", impressoras[i].id, doc.nome);
            }
        }
    }
}

// Exibir status das impressoras
void exibir_status_impressoras(Impressora impressoras[], int num_impressoras) {
    printf("\n=== STATUS DAS IMPRESSORAS ===\n");
    for (int i = 0; i < num_impressoras; i++) {
        printf("Impressora %d: ", impressoras[i].id);
        
        if (impressoras[i].disponivel) {
            printf("DISPONIVEL");
        } else {
            printf("IMPRIMINDO '%s' (ID: %d)", 
                   impressoras[i].documento_atual.nome, 
                   impressoras[i].documento_atual.id);
        }
        
        printf(" | Documentos impressos: %d\n", impressoras[i].documentos_impressos);
    }
    printf("==============================\n");
}

// Adicionar alguns documentos de exemplo
void adicionar_exemplos(FilaPrioridade* fila, int* documento_counter) {
    Documento exemplos[5] = {
        {0, "Relatorio Financeiro", URGENTE, 10},
        {0, "Curriculo Joao Silva", NORMAL, 2},
        {0, "Contrato Cliente X", URGENTE, 5},
        {0, "Artigo Cientifico", BAIXA, 15},
        {0, "Apresentacao Reuniao", NORMAL, 8}
    };
    
    for (int i = 0; i < 5; i++) {
        (*documento_counter)++;
        exemplos[i].id = *documento_counter;
        inserir_na_fila(fila, exemplos[i]);
    }
    
    printf("5 documentos de exemplo adicionados a fila!\n");
}

// Menu principal
void exibir_menu() {
    printf("\n=== SIMULADOR DE IMPRESSORAS EM REDE ===\n");
    printf("1. Adicionar documento\n");
    printf("2. Exibir fila de documentos\n");
    printf("3. Exibir status das impressoras\n");
    printf("4. Processar documentos (1 ciclo)\n");
    printf("5. Adicionar documentos de exemplo\n");
    printf("6. Limpar fila\n");
    printf("7. Sair\n");
    printf("========================================\n");
    printf("Opcao: ");
}

// Função principal
int main() {
    FilaPrioridade fila_central;
    Impressora impressoras[MAX_IMPRESSORAS];
    int documento_counter = 0;
    int num_impressoras = 3;
    
    // Inicializar fila
    inicializar_fila(&fila_central);
    
    // Inicializar impressoras
    for (int i = 0; i < num_impressoras; i++) {
        impressoras[i].id = i + 1;
        impressoras[i].disponivel = 1;
        impressoras[i].documentos_impressos = 0;
        impressoras[i].documento_atual.id = 0;
    }
    
    printf("=== SIMULADOR DE IMPRESSORAS EM REDE ===\n");
    printf("Sistema iniciado com %d impressoras\n", num_impressoras);
    
    int opcao;
    do {
        exibir_menu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                adicionar_documento(&fila_central, &documento_counter);
                break;
                
            case 2:
                exibir_fila(&fila_central);
                break;
                
            case 3:
                exibir_status_impressoras(impressoras, num_impressoras);
                break;
                
            case 4:
                processar_documentos(&fila_central, impressoras, num_impressoras);
                break;
                
            case 5:
                adicionar_exemplos(&fila_central, &documento_counter);
                break;
                
            case 6:
                // Limpar fila
                while (fila_central.front != NULL) {
                    remover_da_fila(&fila_central);
                }
                printf("Fila limpa!\n");
                break;
                
            case 7:
                printf("\n=== RELATORIO FINAL ===\n");
                printf("Documentos na fila: %d\n", fila_central.tamanho);
                
                for (int i = 0; i < num_impressoras; i++) {
                    printf("Impressora %d imprimiu %d documentos\n", 
                           impressoras[i].id, impressoras[i].documentos_impressos);
                }
                
                printf("Simulacao encerrada. Obrigado!\n");
                break;
                
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 7);
    
    return 0;
}