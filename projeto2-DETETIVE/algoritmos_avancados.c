// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// ===============================
// ===== Estrutura do Mapa =======
// ===============================

/*
Struct: Sala
Representa cada cômodo da mansão.
Cada sala possui:
- nome
- ponteiro para esquerda
- ponteiro para direita
*/
typedef struct Sala {
    char nome[50];
    char pista[100];  // NOVO
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

/*
Função: criarSala()
Cria dinamicamente uma nova sala com o nome informado.
Inicializa os ponteiros como NULL.
*/
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));

    if (!nova) {
        printf("Erro de memória!\n");
        exit(1);
    }

    strcpy(nova->nome, nome);

    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        nova->pista[0] = '\0';

    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

// ===============================
// ===== Estrutura da BST ========
// ===============================

typedef struct PistaNode {
    char texto[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Insere pista na BST
PistaNode* inserirPista(PistaNode* raiz, const char* texto) {
    if (!raiz) {
        PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));
        if (!novo) exit(1);
        strcpy(novo->texto, texto);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }

    if (strcmp(texto, raiz->texto) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    else if (strcmp(texto, raiz->texto) > 0)
        raiz->direita = inserirPista(raiz->direita, texto);

    return raiz;
}

// Exibe pistas em ordem alfabética
void exibirPistas(PistaNode* raiz) {
    if (raiz) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->texto);
        exibirPistas(raiz->direita);
    }
}

// ===============================
// ===== TABELA HASH =============
// ===============================
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode* prox;
} HashNode;

HashNode* tabelaHash[TAM_HASH];

/* Função simples de hash */
int funcaoHash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

/* inserirNaHash()
Associa pista a suspeito */
void inserirNaHash(const char* pista, const char* suspeito) {
    int indice = funcaoHash(pista);

    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);

    novo->prox = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

/* encontrarSuspeito()
Consulta suspeito associado à pista */
char* encontrarSuspeito(const char* pista) {
    int indice = funcaoHash(pista);
    HashNode* atual = tabelaHash[indice];

    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}


// ===============================
// ===== Exploração ==============
// ===============================
/*
Função: explorarSalas()
Permite que o jogador explore a mansão a partir da sala atual.
O jogador pode escolher:
e - ir para esquerda
d - ir para direita
s - sair do jogo

A exploração continua até:
- o jogador escolher sair
- chegar em uma sala sem filhos (nó-folha)
*/
void explorarSalas(Sala* atual, PistaNode** arvorePistas) {
    char opcao;

    while (atual) {

        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
            atual->pista[0] = '\0';
        }

        printf("\ne - esquerda | d - direita | s - sair\n");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda)
            atual = atual->esquerda;
        else if (opcao == 'd' && atual->direita)
            atual = atual->direita;
        else if (opcao == 's')
            break;
        else
            printf("Caminho inválido!\n");
    }
}

// ===============================
// ===== Julgamento Final ========
// ===============================

/* verificarSuspeitoFinal()
Conta quantas pistas apontam para o suspeito acusado */
void verificarSuspeitoFinal(PistaNode* raiz, const char* acusado, int* contador) {
    if (!raiz) return;

    verificarSuspeitoFinal(raiz->esquerda, acusado, contador);

    char* suspeito = encontrarSuspeito(raiz->texto);
    if (suspeito && strcmp(suspeito, acusado) == 0)
        (*contador)++;

    verificarSuspeitoFinal(raiz->direita, acusado, contador);
}

// ===============================
// ===== Liberação de Memória ====
// ===============================
/*
Função: liberarMapa()
Libera toda a memória alocada para a árvore.
*/
void liberarMapa(Sala* raiz) {
    if (raiz) {
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        free(raiz);
    }
}

void liberarBST(PistaNode* raiz) {
    if (raiz) {
        liberarBST(raiz->esquerda);
        liberarBST(raiz->direita);
        free(raiz);
    }
}


    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().   

// ===============================
// ===== MAIN ====================
// ===============================

/*
Função principal: main()

- Monta manualmente o mapa da mansão
- Inicia a exploração a partir do Hall de Entrada
*/

int main() {

    // Inicializa tabela hash
    for (int i = 0; i < TAM_HASH; i++)
        tabelaHash[i] = NULL;

    // Associa pistas a suspeitos
    inserirNaHash("Pegadas de barro", "Carlos");
    inserirNaHash("Copo quebrado", "Ana");
    inserirNaHash("Faca ensanguentada", "Carlos");
    inserirNaHash("Livro fora da estante", "Marina");
    inserirNaHash("Carta misteriosa", "Carlos");

    // Monta mapa fixo
    Sala* hall = criarSala("Hall de Entrada", "Pegadas de barro");
    Sala* salaEstar = criarSala("Sala de Estar", "Copo quebrado");
    Sala* cozinha = criarSala("Cozinha", "Faca ensanguentada");
    Sala* biblioteca = criarSala("Biblioteca", "Livro fora da estante");
    Sala* quarto = criarSala("Quarto", "Carta misteriosa");

    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    cozinha->esquerda = quarto;

    PistaNode* pistas = NULL;

    printf("=== Detective Quest - Nivel Mestre ===\n");

    explorarSalas(hall, &pistas);

    printf("\n=== Pistas Coletadas ===\n");
    exibirPistas(pistas);

// === ACUSAÇÃO FINAL ===
char acusado[50];
int escolha;

printf("\n=== SUSPEITOS DISPONÍVEIS ===\n");
printf("1 - Carlos\n");
printf("2 - Ana\n");
printf("3 - Marina\n");

printf("\nEscolha o número do suspeito: ");
scanf("%d", &escolha);

switch (escolha) {
    case 1:
        strcpy(acusado, "Carlos");
        break;
    case 2:
        strcpy(acusado, "Ana");
        break;
    case 3:
        strcpy(acusado, "Marina");
        break;
    default:
        printf("Opção inválida!\n");
        return 0;
}

printf("\nVocê acusou: %s\n", acusado);

// Aqui continua sua lógica de verificação
    if (strcmp(acusado, "Carlos") == 0) {
     printf("Carlos é o culpado!\n");
    } else {
     printf("%s é inocente!\n", acusado);
    }

return 0;
}