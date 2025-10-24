/**
 * @file inventario.c
 * @brief Simulação de um sistema de inventário/mochila de loot inicial
 * utilizando structs e uma lista sequencial (vetor de structs) em C.
 * * Requisitos: Implementar funções para Inserir, Remover, Listar e Buscar Itens.
 * Capacidade máxima da mochila: 10 itens.
 */

#include <stdio.h>
#include <stdlib.h> // Para a função exit()
#include <string.h>

// Definindo a capacidade máxima do inventário (mochila)
#define MAX_INVENTARIO 10
#define MAX_NOME 50
#define MAX_TIPO 20

// Variável global para rastrear o número atual de itens na mochila (tamanho da lista).
int contador_itens = 0;

// -----------------------------------------------------------------------------
// 1. Definição da struct (Requisito Funcional)
// -----------------------------------------------------------------------------
/**
 * @struct Item
 * @brief Estrutura composta para representar um item no inventário.
 * * Campos:
 * - nome: Nome do item (ex: Faca, Kit Médico).
 * - tipo: Categoria do item (ex: Arma, Ferramenta, Médico).
 * - peso: O peso do item (para futuras expansões de limite de peso).
 * - quantidade: O número de unidades deste item.
 */
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    float peso;
    int quantidade;
} Item;

// -----------------------------------------------------------------------------
// 2. Funções obrigatórias
// -----------------------------------------------------------------------------

/**
 * @brief Adiciona um novo item ao inventário, se houver espaço.
 * @param inventario O vetor de structs que representa a mochila.
 * * Atende ao requisito de Usabilidade com mensagens claras.
 */
void inserirItem(Item inventario[]) {
    if (contador_itens >= MAX_INVENTARIO) {
        printf("\n[MOCHILA CHEIA!] Capacidade máxima (%d) atingida. Remova um item primeiro.\n", MAX_INVENTARIO);
        return;
    }

    Item novo_item;
    printf("\n--- Cadastrar Novo Item ---\n");

    // Entrada do Nome
    printf("Nome do Item (max %d caracteres): ", MAX_NOME - 1);
    // Limpa o buffer de entrada antes de usar fgets
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    
    // Leitura segura do nome
    if (fgets(novo_item.nome, sizeof(novo_item.nome), stdin) != NULL) {
        // Remove o '\n' do fgets
        novo_item.nome[strcspn(novo_item.nome, "\n")] = '\0';
    }

    // Entrada do Tipo
    printf("Tipo (ex: Arma, Medico, Ferramenta): ");
    scanf("%s", novo_item.tipo);

    // Entrada do Peso
    printf("Peso (em kg, use . para decimais): ");
    scanf("%f", &novo_item.peso);

    // Entrada da Quantidade
    printf("Quantidade: ");
    scanf("%d", &novo_item.quantidade);

    // Adiciona o item na próxima posição livre e incrementa o contador.
    inventario[contador_itens] = novo_item;
    contador_itens++;
    
    printf("\n[SUCESSO] '%s' (%s) adicionado à mochila. Espaço restante: %d.\n", 
           novo_item.nome, novo_item.tipo, MAX_INVENTARIO - contador_itens);
}

/**
 * @brief Remove um item do inventário com base no seu índice.
 * @param inventario O vetor de structs que representa a mochila.
 */
void removerItem(Item inventario[]) {
    if (contador_itens == 0) {
        printf("\n[AVISO] A mochila está vazia. Nada a remover.\n");
        return;
    }

    int indice_remover;
    listarItens(inventario); // Mostra a lista para auxiliar o usuário
    
    printf("\n--- Remover Item ---\n");
    printf("Digite o número do item (índice) que deseja remover (1 a %d): ", contador_itens);
    scanf("%d", &indice_remover);
    
    // Ajusta o índice para base zero
    indice_remover--;

    if (indice_remover < 0 || indice_remover >= contador_itens) {
        printf("\n[ERRO] Índice inválido. Tente novamente.\n");
        return;
    }

    char nome_removido[MAX_NOME];
    strcpy(nome_removido, inventario[indice_remover].nome);

    // Movimenta os elementos seguintes para cobrir a posição removida
    // (Operação essencial de lista sequencial/vetor)
    for (int i = indice_remover; i < contador_itens - 1; i++) {
        inventario[i] = inventario[i + 1];
    }

    contador_itens--;
    printf("\n[SUCESSO] Item '%s' removido da mochila.\n", nome_removido);
}

/**
 * @brief Exibe todos os itens atualmente no inventário.
 * @param inventario O vetor de structs que representa a mochila.
 */
void listarItens(Item inventario[]) {
    printf("\n=================================================================\n");
    printf("                     INVENTÁRIO (MOCHILA)                          \n");
    printf("=================================================================\n");
    
    if (contador_itens == 0) {
        printf("|                   A MOCHILA ESTÁ VAZIA!                       |\n");
    } else {
        printf("| %-5s | %-20s | %-10s | %-5s | %-8s |\n", "ID", "NOME", "TIPO", "QTD", "PESO (kg)");
        printf("-----------------------------------------------------------------\n");
        // Loop 'for' para percorrer e exibir cada item
        for (int i = 0; i < contador_itens; i++) {
            printf("| %-5d | %-20s | %-10s | %-5d | %-8.2f |\n", 
                   i + 1, 
                   inventario[i].nome, 
                   inventario[i].tipo, 
                   inventario[i].quantidade, 
                   inventario[i].peso);
        }
    }
    printf("=================================================================\n");
}

/**
 * @brief Busca por um item no inventário pelo nome.
 * @param inventario O vetor de structs que representa a mochila.
 */
void buscarItem(Item inventario[]) {
    if (contador_itens == 0) {
        printf("\n[AVISO] A mochila está vazia. Não há itens para buscar.\n");
        return;
    }
    
    char nome_busca[MAX_NOME];
    printf("\n--- Buscar Item por Nome ---\n");
    printf("Digite o nome do item a buscar: ");
    
    // Limpa o buffer de entrada antes de usar scanf para string
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    
    // Leitura segura do nome a buscar
    if (fgets(nome_busca, sizeof(nome_busca), stdin) != NULL) {
        // Remove o '\n' do fgets
        nome_busca[strcspn(nome_busca, "\n")] = '\0';
    }

    int encontrado = 0;
    
    // Laço 'for' para percorrer a lista
    for (int i = 0; i < contador_itens; i++) {
        // Comparação de strings com strcasecmp ou stricmp (não padrão) ou strcmp (case sensitive)
        // Usaremos strcmp para simplicidade.
        if (strcmp(inventario[i].nome, nome_busca) == 0) {
            printf("\n[ENCONTRADO!] Item '%s' na posição %d (Tipo: %s, Qtd: %d, Peso: %.2f).\n", 
                   inventario[i].nome, 
                   i + 1,
                   inventario[i].tipo,
                   inventario[i].quantidade,
                   inventario[i].peso);
            encontrado = 1;
            break; // Item encontrado, podemos sair do loop
        }
    }

    if (!encontrado) {
        printf("\n[NÃO ENCONTRADO] O item '%s' não está na mochila.\n", nome_busca);
    }
}

// -----------------------------------------------------------------------------
// 3. Função Principal e Menu (Usabilidade)
// -----------------------------------------------------------------------------

/**
 * @brief Função principal: gerencia o loop do menu e chama as funções de inventário.
 */
int main() {
    // 3. Vetor de structs: Inventário (mochila)
    Item mochila[MAX_INVENTARIO];
    int opcao;

    printf("====================================================\n");
    printf("         BEM-VINDO À SIMULAÇÃO DE INVENTÁRIO        \n");
    printf("====================================================\n");

    // Loop principal do menu
    do {
        // Exibição do menu de opções (Usabilidade)
        printf("\n[MOCHILA] Itens: %d/%d\n", contador_itens, MAX_INVENTARIO);
        printf("--- MENU DE AÇÕES ---\n");
        printf("1. Coletar Item (Inserir)\n");
        printf("2. Descartar Item (Remover)\n");
        printf("3. Inspecionar Mochila (Listar)\n");
        printf("4. Procurar por Item (Buscar)\n");
        printf("0. Sair do Jogo\n");
        printf("Escolha uma opção: ");
        
        // Leitura da opção
        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Opção inválida. Digite um número.\n");
            // Limpa o buffer de entrada para evitar loop infinito
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            continue;
        }

        // Tratamento da opção escolhida
        switch (opcao) {
            case 1:
                inserirItem(mochila);
                break;
            case 2:
                removerItem(mochila);
                break;
            case 3:
                listarItens(mochila);
                break;
            case 4:
                buscarItem(mochila);
                break;
            case 0:
                printf("\nSaindo do jogo... Boa sorte na próxima missão!\n");
                break;
            default:
                printf("\n[AVISO] Opção não reconhecida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}