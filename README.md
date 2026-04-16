# Estrutura-de-dados
Este repositório conta com 4 projetos desenvolvidos na disciplina de Estrutura de dados, onde foram desenvolvidos modelos de jogos clássicos utilizando estruturas de dados para aprendizado.

------------------------------------------------------------------------------------------------------------------
Para o jogo WAR foi utilizado: struct, vetor, entrada/saída, ponteiros, memória dinâmica, lógica de jogo,  modularização, design limpo e um sistema de missões.


------------------------------------------------------------------------------------------------------------------
Para o jogo DETETIVE foram utilizadas estruturas fundamentais como árvores e tabelas hash para controlar lógica de jogo


------------------------------------------------------------------------------------------------------------------
Foi feito um SISTEMA DE ARMAZENAMENTO utilizando o jogo FREE FIRE: 
  - Simulação o gerenciamento de um inventário de sobrevivência
  - Conceitos utilizados: Manipulação de estruturas e arrays; Criação de menus interativos; Implementação de buscas e ordenações; Pensamento modular e boas práticas de software.
  - Funcionalidades
    Adicionar itens à mochila (nome, tipo e quantidade)
    Remover itens pelo nome
    Listar os itens cadastrados
    Busca sequencial
    Comparação de strings (strcmp)
    Controle com flag para indicar se item foi encontrado
    Enumeração (enum) para critérios de ordenação
    Ordenação com Insertion Sort
    Busca binária (binary search)
    Uso de bool para controle de estado
    Análise de desempenho com contador de comparações.


------------------------------------------------------------------------------------------------------------------
Para o jogo TETRIS foi desenvolvido sistema de gerenciamento de peças, utilizando estruturas como fila circular e pilha.
  Funcionalidades do Sistema:
    Inicializar a fila com 5 peças geradas automaticamente.
    Permitir que o usuário:
      Jogue uma peça (dequeue)
      Insira uma nova peça (enqueue)
      Visualize o estado atual da fila
      Manter a fila circular, reaproveitando o espaço.
    O usuário pode:
      Reservar a peça da frente da fila (push)
      Usar uma peça reservada (pop)
      A fila continua funcionando com inserção automática.
      A pilha tem capacidade máxima de 3 peças.
    Menu com múltiplas ações:
      1 - Jogar peça
      2 - Reservar peça
      3 - Usar peça reservada
      4 - Trocar peça do topo da pilha com a da frente da fila
      5 - Desfazer última jogada
      6 - Inverter fila com pilha
      0 - Sair
    Controle de fila circular e pilha de reserva com atualização a cada ação.
      
