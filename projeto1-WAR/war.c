// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================

// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:
Territorio* alocarMapa(int quantidade);
void cadastrarTerritorios(Territorio* mapa, int quantidade);
void exibirMapa(const Territorio* mapa, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa, char* missaoJogador);

void atribuirMissao(char* destino, const char* missoes[], int totalMissoes);
int verificarMissao(const char* missao, const char* corJogador, Territorio* mapa, int tamanho);
void exibirMissao(const char* missao);
int lerInteiro(const char* mensagem);

// ================= UTIL =================
int lerInteiro(const char* mensagem) {
    int valor;
    char buffer[100];

    while (1) {
        printf("%s", mensagem);

        if (fgets(buffer, sizeof(buffer), stdin) != NULL &&
            sscanf(buffer, "%d", &valor) == 1)
            return valor;

        printf("Entrada invalida! Digite um numero inteiro.\n");
    }
}

// ================= MAIN =================
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    srand(time(NULL));

    int quantidade;
    do {
    quantidade = lerInteiro("Quantos territorios deseja criar? (minimo 2): ");
    if (quantidade < 2)
        printf("O jogo precisa de pelo menos 2 territorios!\n");
    } while (quantidade < 2);

    Territorio* mapa = alocarMapa(quantidade);
    cadastrarTerritorios(mapa, quantidade);

    char corJogador[10];
    strncpy(corJogador, mapa[0].cor, sizeof(corJogador)-1);
    corJogador[sizeof(corJogador)-1] = '\0';

    // ---------- MISSOES ----------
  const char* listaMissoes[] = {
        "Conquistar 3 territorios",
        "Possuir 10 tropas em um territorio",
        "Dominar todos os territorios",
    };

    char* missaoJogador = (char*) malloc(100);
    if (!missaoJogador) { printf("Erro de memoria!\n"); exit(1);}

    atribuirMissao(missaoJogador, listaMissoes, 3);
    exibirMissao(missaoJogador);
    
    int opcao = -1;

    printf("\nO jogo vai comecar!\n");

    while (opcao != 0) {

        printf("\n=========== MAPA ATUAL ===========\n");
        exibirMapa(mapa, quantidade);

        // MOSTRAR MISSÃO TODO TURNO
        exibirMissao(missaoJogador);

        opcao = lerInteiro("\n1 - Atacar\n0 - Sair\nEscolha: ");

        if (opcao == 1) {

            int a, d;
            a = lerInteiro("\nEscolha o territorio ATACANTE (numero): ");
            d = lerInteiro("Escolha o territorio DEFENSOR (numero): ");
            
            if (a < 1 || a > quantidade || d < 1 || d > quantidade) {
                printf("Territorio invalido!\n");
                continue;
            }

            if (a == d) {
                printf("Um territorio nao pode atacar ele mesmo!\n");
                continue;
            }       
                
            if (strcmp(mapa[a-1].cor, mapa[d-1].cor) == 0) {
                printf("Nao pode atacar territorio da mesma cor!\n");
                continue;
            }

            if (mapa[a-1].tropas <= 1) {
                printf("Territorio atacante nao possui tropas suficientes!\n");
                continue;
            }

            atacar(&mapa[a-1], &mapa[d-1]);

            // checagem silenciosa de vitoria
            if (verificarMissao(missaoJogador, corJogador, mapa, quantidade)) {
                printf("\n*** MISSAO CUMPRIDA! VOCE VENCEU! ***\n");
                break;
            }
        }
    }

    liberarMemoria(mapa, missaoJogador);
    return 0;
}

    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

// --- Implementação das Funções ---

// ---------------------------------------------------------------------------
// ALOCAÇÃO
// ---------------------------------------------------------------------------
// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
Territorio* alocarMapa(int quantidade) {
    Territorio* mapa = (Territorio*) calloc(quantidade, sizeof(Territorio));

    if (!mapa) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
    return mapa;
}

// ---------------------------------------------------------------------------
// CADASTRO DE TERRITORIOS
// ---------------------------------------------------------------------------
// cadastrarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void cadastrarTerritorios(Territorio* mapa, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\nTerritorio %d\n", i+1);

        printf("Nome: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        do {
            mapa[i].tropas = lerInteiro("Tropas (min 1): ");
        } while (mapa[i].tropas < 1);
    }
}

// ---------------------------------------------------------------------------
// EXIBIÇÃO DO MAPA
// ---------------------------------------------------------------------------
// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio* mapa, int quantidade) {
    for (int i = 0; i < quantidade; i++) 
        printf("\n[%d] %s | Dono: %s | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
}

// ---------------------------------------------------------------------------
// ATAQUE (REGRA PRINCIPAL)
// ---------------------------------------------------------------------------
// atacar():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n--- BATALHA ---\n");
    printf("%s (%s) rolou: %d\n", atacante->nome, atacante->cor, dadoAtacante);
    printf("%s (%s) rolou: %d\n", defensor->nome, defensor->cor, dadoDefensor);

    // empate favorece defensor (regra oficial do WAR)
    if (dadoAtacante > dadoDefensor) {

        printf("ATACANTE VENCEU E CONQUISTOU O TERRITORIO!\n");

        int tropasEnviadas = atacante->tropas / 2;

        if (tropasEnviadas < 1)
            tropasEnviadas = 1;

        if (tropasEnviadas >= atacante->tropas)
            tropasEnviadas = atacante->tropas - 1;

        atacante->tropas -= tropasEnviadas;

        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = tropasEnviadas;
    }
    else {
        printf("DEFENSOR RESISTIU AO ATAQUE!\n");
        atacante->tropas--;

        if (atacante->tropas < 1)
            atacante->tropas = 1;
    }
}

// ================= MISSOES =================
// atribuirMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strncpy(destino, missoes[sorteio], 99);
    destino[99] = '\0';
}

void exibirMissao(const char* missao) {
    printf("\n===== SUA MISSAO =====\n%s\n======================\n", missao);
}

int verificarMissao(const char* missao, const char* corJogador, Territorio* mapa, int tamanho) {
    int contador = 0;

    if (strcmp(missao, "Conquistar 3 territorios") == 0) {
        for (int i=0;i<tamanho;i++)
            if (strcmp(mapa[i].cor, corJogador)==0)
                contador++;
        return contador>=3;
    }

    if (strcmp(missao, "Possuir 10 tropas em um territorio") == 0) {
        for (int i=0;i<tamanho;i++)
            if (strcmp(mapa[i].cor, corJogador)==0 && mapa[i].tropas>=10)
                return 1;
    }

    if (strcmp(missao, "Dominar todos os territorios") == 0) {
        for (int i=1;i<tamanho;i++)
            if (strcmp(mapa[i].cor, corJogador)!=0)
                return 0;
        return 1;
    }

    return 0;
}

// ================= LIMPEZA =================
void liberarMemoria(Territorio* mapa, char* missaoJogador) {
free(mapa);
free(missaoJogador);
}
