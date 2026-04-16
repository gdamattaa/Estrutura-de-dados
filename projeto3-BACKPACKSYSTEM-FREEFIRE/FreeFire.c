#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 20

// =========================
// STRUCT
// =========================
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

// =========================
// LISTA ENCADEADA
// =========================
typedef struct No {
    Componente dados;
    struct No* proximo;
} No;

// =========================
// VARIÁVEIS GLOBAIS
// =========================
Componente vetor[MAX];
int total = 0;
No* lista = NULL;
long comparacoes = 0;

// =========================
// AUXILIARES
// =========================
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void mostrarVetor() {
    if (total == 0) {
        printf("Vetor vazio.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        printf("%d. %s | %s | Prioridade: %d\n",
               i+1,
               vetor[i].nome,
               vetor[i].tipo,
               vetor[i].prioridade);
    }
}

void mostrarLista() {
    if (!lista) {
        printf("Lista vazia.\n");
        return;
    }

    No* atual = lista;
    int i = 1;

    while (atual) {
        printf("%d. %s | %s | Prioridade: %d\n",
               i++,
               atual->dados.nome,
               atual->dados.tipo,
               atual->dados.prioridade);
        atual = atual->proximo;
    }
}

// =========================
// INSERÇÃO
// =========================
void inserirVetor() {
    if (total >= MAX) {
        printf("Vetor cheio!\n");
        return;
    }

    printf("Nome: ");
    fgets(vetor[total].nome, 30, stdin);
    vetor[total].nome[strcspn(vetor[total].nome, "\n")] = 0;

    printf("Tipo: ");
    fgets(vetor[total].tipo, 20, stdin);
    vetor[total].tipo[strcspn(vetor[total].tipo, "\n")] = 0;

    printf("Prioridade: ");
    scanf("%d", &vetor[total].prioridade);
    limparBuffer();

    total++;
}

void inserirLista() {
    No* novo = malloc(sizeof(No));

    printf("Nome: ");
    fgets(novo->dados.nome, 30, stdin);
    novo->dados.nome[strcspn(novo->dados.nome, "\n")] = 0;

    printf("Tipo: ");
    fgets(novo->dados.tipo, 20, stdin);
    novo->dados.tipo[strcspn(novo->dados.tipo, "\n")] = 0;

    printf("Prioridade: ");
    scanf("%d", &novo->dados.prioridade);
    limparBuffer();

    novo->proximo = lista;
    lista = novo;
}

// =========================
// BUSCA SEQUENCIAL
// =========================
int buscaSequencialVetor(char chave[]) {
    comparacoes = 0;
    for (int i = 0; i < total; i++) {
        comparacoes++;
        if (strcmp(vetor[i].nome, chave) == 0)
            return i;
    }
    return -1;
}

No* buscaSequencialLista(char chave[]) {
    comparacoes = 0;
    No* atual = lista;
    while (atual) {
        comparacoes++;
        if (strcmp(atual->dados.nome, chave) == 0)
            return atual;
        atual = atual->proximo;
    }
    return NULL;
}

// =========================
// ORDENAÇÕES (VETOR)
// =========================
void bubbleSortNome() {
    comparacoes = 0;
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            comparacoes++;
            if (strcmp(vetor[j].nome, vetor[j+1].nome) > 0) {
                Componente temp = vetor[j];
                vetor[j] = vetor[j+1];
                vetor[j+1] = temp;
            }
        }
    }
}

void insertionSortTipo() {
    comparacoes = 0;
    for (int i = 1; i < total; i++) {
        Componente chave = vetor[i];
        int j = i - 1;

        while (j >= 0 && strcmp(vetor[j].tipo, chave.tipo) > 0) {
            comparacoes++;
            vetor[j+1] = vetor[j];
            j--;
        }
        comparacoes++;
        vetor[j+1] = chave;
    }
}

void selectionSortPrioridade() {
    comparacoes = 0;
    for (int i = 0; i < total - 1; i++) {
        int min = i;
        for (int j = i + 1; j < total; j++) {
            comparacoes++;
            if (vetor[j].prioridade < vetor[min].prioridade)
                min = j;
        }

        if (min != i) {
            Componente temp = vetor[i];
            vetor[i] = vetor[min];
            vetor[min] = temp;
        }
    }
}

// =========================
// BUSCA BINÁRIA (NOME)
// =========================
int buscaBinaria(char chave[]) {
    int inicio = 0, fim = total - 1;
    comparacoes = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        comparacoes++;

        int cmp = strcmp(vetor[meio].nome, chave);

        if (cmp == 0)
            return meio;
        else if (cmp < 0)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }
    return -1;
}

// =========================
// MEDIR TEMPO
// =========================
double medirTempo(void (*func)()) {
    clock_t ini = clock();
    func();
    clock_t fim = clock();
    return (double)(fim - ini) / CLOCKS_PER_SEC;
}

// =========================
// MAIN
// =========================
int main() {

    int estrutura, opcao;

    printf("=== TORRE FINAL - NIVEL MESTRE INTEGRADO ===\n");
    printf("Escolha estrutura:\n1 - Vetor\n2 - Lista\n");
    scanf("%d", &estrutura);
    limparBuffer();

    do {
        printf("\n1 - Inserir\n");
        printf("2 - Mostrar\n");
        printf("3 - Busca Sequencial\n");

        if (estrutura == 1) {
            printf("4 - Bubble (Nome)\n");
            printf("5 - Insertion (Tipo)\n");
            printf("6 - Selection (Prioridade)\n");
            printf("7 - Busca Binaria (Nome)\n");
        }

        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        char nome[30];
        double tempo;

        if (estrutura == 1) {
            switch (opcao) {
                case 1: inserirVetor(); break;
                case 2: mostrarVetor(); break;

                case 3:
                    printf("Nome: ");
                    fgets(nome,30,stdin);
                    nome[strcspn(nome,"\n")] = 0;
                    if (buscaSequencialVetor(nome) != -1)
                        printf("Encontrado! Comparacoes: %ld\n", comparacoes);
                    else
                        printf("Nao encontrado.\n");
                    break;

                case 4:
                    tempo = medirTempo(bubbleSortNome);
                    printf("Ordenado por Nome. Comp: %ld Tempo: %f\n", comparacoes, tempo);
                    break;

                case 5:
                    tempo = medirTempo(insertionSortTipo);
                    printf("Ordenado por Tipo. Comp: %ld Tempo: %f\n", comparacoes, tempo);
                    break;

                case 6:
                    tempo = medirTempo(selectionSortPrioridade);
                    printf("Ordenado por Prioridade. Comp: %ld Tempo: %f\n", comparacoes, tempo);
                    break;

                case 7:
                    printf("Nome: ");
                    fgets(nome,30,stdin);
                    nome[strcspn(nome,"\n")] = 0;
                    if (buscaBinaria(nome) != -1)
                        printf("Encontrado via Binaria! Comp: %ld\n", comparacoes);
                    else
                        printf("Nao encontrado.\n");
                    break;
            }
        } else {
            switch (opcao) {
                case 1: inserirLista(); break;
                case 2: mostrarLista(); break;
                case 3:
                    printf("Nome: ");
                    fgets(nome,30,stdin);
                    nome[strcspn(nome,"\n")] = 0;
                    if (buscaSequencialLista(nome))
                        printf("Encontrado! Comparacoes: %ld\n", comparacoes);
                    else
                        printf("Nao encontrado.\n");
                    break;
            }
        }

    } while (opcao != 0);

    return 0;
}
