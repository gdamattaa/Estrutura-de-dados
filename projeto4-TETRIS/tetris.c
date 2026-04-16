#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha
// Este código inicial serve como base para o desenvolvimento do sistema de controle de peças.
// Use as instruções de cada nível para desenvolver o desafio.

#define MAX_FILA 5
#define MAX_PILHA 3


// =================================
// Estrutura que representa uma peça
// =================================
typedef struct {
    char tipo;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;


// ==========================
// Estrutura da Fila Circular
// ==========================
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int tamanho;
} Fila;


// ==========================
// Estrutura da Pilha
// ==========================
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;


// ==========================================
// Variável global para gerar IDs sequenciais
// ==========================================
int proximoId = 0;

// ==========================================
// Função para gerar uma peça automaticamente
// ==========================================
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;

    nova.tipo = tipos[rand() % 4];  //Sorteia tipo
    nova.id = proximoId++;          //ID sequencial

    return nova;
}


// ==========================
// FUNÇÕES DA FILA
// ==========================
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

void enqueue(Fila *f, Peca p) {
    if (f->tamanho == MAX_FILA) return;

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->tamanho++;
}

Peca dequeue(Fila *f) {
    Peca removida = {'?', -1};

    if (filaVazia(f)) return removida;

    removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->tamanho--;

    return removida;
}

Peca acessarFila(Fila *f, int pos) {
    int indice = (f->inicio + pos) % MAX_FILA;
    return f->itens[indice];
}

void definirFila(Fila *f, int pos, Peca valor) {
    int indice = (f->inicio + pos) % MAX_FILA;
    f->itens[indice] = valor;
}

// ==========================
// FUNÇÕES DA PILHA
// ==========================
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

void push(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return;
    p->itens[++p->topo] = nova;
}

Peca pop(Pilha *p) {
    Peca removida = {'?', -1};
    if (pilhaVazia(p)) return removida;
    return p->itens[p->topo--];
}

// ==========================
// EXIBIÇÃO
// ==========================
void mostrarFila(Fila *f) {
    printf("\nFila de peças:\n");
    for (int i = 0; i < f->tamanho; i++) {
        Peca p = acessarFila(f, i);
        printf("[%c %d] ", p.tipo, p.id);
    }
    printf("\n");
}

void mostrarPilha(Pilha *p) {
    printf("\nPilha de reserva (Topo -> Base):\n");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
    }
    if (pilhaVazia(p)) printf("[vazia]");
    printf("\n");
}

void mostrarEstado(Fila *f, Pilha *p) {
    printf("\n=================================");
    printf("\nEstado atual:");
    printf("\n=================================");
    mostrarFila(f);
    mostrarPilha(p);
    printf("=================================\n");
}

// ==========================
// TROCA SIMPLES
// ==========================
void trocarTopoComFrente(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\n⚠ Não é possível trocar.\n");
        return;
    }

    int indiceFrente = f->inicio;

    Peca temp = f->itens[indiceFrente];
    f->itens[indiceFrente] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("\nTroca realizada entre frente da fila e topo da pilha.\n");
}

// ==========================
// TROCA MÚLTIPLA (3 POR 3)
// ==========================
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->tamanho < 3 || p->topo < 2) {
        printf("\n⚠ Não há peças suficientes para troca múltipla.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int indiceFila = (f->inicio + i) % MAX_FILA;

        Peca temp = f->itens[indiceFila];
        f->itens[indiceFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }

    printf("\nAção: troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

// ==========================
// MAIN
// ==========================
int main() {

    Fila fila;
    Pilha pilha;
    int opcao;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa fila com 5 peças
    for (int i = 0; i < MAX_FILA; i++)
        enqueue(&fila, gerarPeca());

    do {
        mostrarEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente\n");
        printf("2 - Enviar peça para a pilha\n");
        printf("3 - Usar peça da pilha\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Troca múltipla (3 por 3)\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1: {
                Peca jogada = dequeue(&fila);
                if (jogada.id != -1) {
                    printf("\nPeça jogada: [%c %d]\n", jogada.tipo, jogada.id);
                    enqueue(&fila, gerarPeca());
                }
                break;
            }

            case 2: {
                if (!pilhaCheia(&pilha)) {
                    Peca reservada = dequeue(&fila);
                    if (reservada.id != -1) {
                        push(&pilha, reservada);
                        enqueue(&fila, gerarPeca());
                        printf("\nPeça enviada para a pilha.\n");
                    }
                } else {
                    printf("\n⚠ Pilha cheia.\n");
                }
                break;
            }

            case 3: {
                Peca usada = pop(&pilha);
                if (usada.id != -1)
                    printf("\nPeça usada da pilha: [%c %d]\n", usada.tipo, usada.id);
                break;
            }

            case 4:
                trocarTopoComFrente(&fila, &pilha);
                break;

            case 5:
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando programa...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}

//=================================================================================================================

    // 🧩 Nível Novato: Fila de Peças Futuras
    //
    // - Crie uma struct Peca com os campos: tipo (char) e id (int).
    // - Implemente uma fila circular com capacidade para 5 peças.
    // - Crie funções como inicializarFila(), enqueue(), dequeue(), filaCheia(), filaVazia().
    // - Cada peça deve ser gerada automaticamente com um tipo aleatório e id sequencial.
    // - Exiba a fila após cada ação com uma função mostrarFila().
    // - Use um menu com opções como:
    //      1 - Jogar peça (remover da frente)
    //      0 - Sair
    // - A cada remoção, insira uma nova peça ao final da fila.


    // 🧠 Nível Aventureiro: Adição da Pilha de Reserva
    //
    // - Implemente uma pilha linear com capacidade para 3 peças.
    // - Crie funções como inicializarPilha(), push(), pop(), pilhaCheia(), pilhaVazia().
    // - Permita enviar uma peça da fila para a pilha (reserva).
    // - Crie um menu com opção:
    //      2 - Enviar peça da fila para a reserva (pilha)
    //      3 - Usar peça da reserva (remover do topo da pilha)
    // - Exiba a pilha junto com a fila após cada ação com mostrarPilha().
    // - Mantenha a fila sempre com 5 peças (repondo com gerarPeca()).


    // 🔄 Nível Mestre: Integração Estratégica entre Fila e Pilha
    //
    // - Implemente interações avançadas entre as estruturas:
    //      4 - Trocar a peça da frente da fila com o topo da pilha
    //      5 - Trocar os 3 primeiros da fila com as 3 peças da pilha
    // - Para a opção 4:
    //      Verifique se a fila não está vazia e a pilha tem ao menos 1 peça.
    //      Troque os elementos diretamente nos arrays.
    // - Para a opção 5:
    //      Verifique se a pilha tem exatamente 3 peças e a fila ao menos 3.
    //      Use a lógica de índice circular para acessar os primeiros da fila.
    // - Sempre valide as condições antes da troca e informe mensagens claras ao usuário.
    // - Use funções auxiliares, se quiser, para modularizar a lógica de troca.
    // - O menu deve ficar assim:
    //      4 - Trocar peça da frente com topo da pilha
    //      5 - Trocar 3 primeiros da fila com os 3 da pilha

