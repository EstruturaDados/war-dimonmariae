#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
#define TAM_NOME 30
#define TAM_COR 10

// --- Definição da Estrutura (Struct) ---
struct Territorio {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
};

// --- Protótipos das Funções ---
void limparBufferEntrada();
struct Territorio* alocarTerritorios(int quantidade);
void cadastrarTerritorios(struct Territorio* territorios, int quantidade);
void exibirMapa(struct Territorio* territorios, int quantidade);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void liberarMemoria(struct Territorio* mapa);
int validarAtaque(struct Territorio* atacante, struct Territorio* defensor);

// --- Função para limpar o buffer de entrada ---
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Função para alocar memória dinamicamente ---
struct Territorio* alocarTerritorios(int quantidade) {
    struct Territorio* territorios = (struct Territorio*)calloc(quantidade, sizeof(struct Territorio));
    if (territorios == NULL) {
        printf("Erro: Não foi possível alocar memória!\n");
        exit(1);
    }
    return territorios;
}

// --- Função para cadastrar territórios ---
void cadastrarTerritorios(struct Territorio* territorios, int quantidade) {
    printf("\n--- Cadastro de %d Territórios ---\n", quantidade);
    
    for (int i = 0; i < quantidade; i++) {
        printf("\n--- Território %d ---\n", i + 1);
        
        // Entrada nome
        printf("Digite o nome do território: ");
        fgets(territorios[i].nome, TAM_NOME, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';
        
        // Entrada cor
        printf("Digite a cor do exército: ");
        fgets(territorios[i].cor, TAM_COR, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';
        
        // Entrada tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
        limparBufferEntrada();
    }
}

// --- Função para exibir o mapa ---
void exibirMapa(struct Territorio* territorios, int quantidade) {
    printf("\n---\n");
    printf("    MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("---\n\n");
    
    for (int i = 0; i < quantidade; i++) {
        printf("%d. %s (%s, Tropas: %d)\n", 
               i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
    printf("\n");
}

// --- Função para validar se o ataque é permitido ---
int validarAtaque(struct Territorio* atacante, struct Territorio* defensor) {
    // Verifica se é o mesmo território
    if (atacante == defensor) {
        printf("Erro: Não é possível atacar o próprio território!\n");
        return 0;
    }
    
    // Verifica se as cores são diferentes
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Erro: Não é possível atacar um território da mesma cor!\n");
        return 0;
    }
    
    // Verifica se o atacante tem tropas suficientes
    if (atacante->tropas <= 1) {
        printf("Erro: O território atacante precisa de pelo menos 2 tropas para atacar!\n");
        return 0;
    }
    
    return 1;
}

// --- Função para simular um ataque ---
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    printf("\n-- RESULTADO DA BATALHA --\n");
    
    // Simula a rolagem de dados (1-6 para cada lado)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;
    
    printf("0 atacante %s rolou um dado e tirou: %d\n", atacante->nome, dadoAtacante);
    printf("0 defensor %s rolou um dado e tirou: %d\n", defensor->nome, dadoDefensor);
    
    // Determina o vencedor da batalha
    if (dadoAtacante > dadoDefensor) {
        printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        
        // O atacante conquista o território
        strcpy(defensor->cor, atacante->cor);
        
        // Metade das tropas do defensor são transferidas (mínimo 1)
        int tropasTransferidas = defensor->tropas / 2;
        if (tropasTransferidas < 1) tropasTransferidas = 1;
        
        defensor->tropas = tropasTransferidas;
        atacante->tropas--; // Atacante perde uma tropa no ataque
        
        printf("CONQUISTA! O territorio %s foi dominado pelo Exercito %s!\n", 
               defensor->nome, defensor->cor);
        
    } else if (dadoAtacante < dadoDefensor) {
        printf("VITORIA DA DEFESA! O atacante perdeu 1 tropa.\n");
        
        // Atacante perde uma tropa
        atacante->tropas--;
        
        printf("O territorio %s se manteve sob dominio do Exercito %s!\n", 
               defensor->nome, defensor->cor);
        
    } else {
        printf("EMPATE! Ambos os territorios perderam 1 tropa.\n");
        
        // Ambos perdem uma tropa no empate
        atacante->tropas--;
        defensor->tropas--;
        
        printf("Nenhum territorio mudou de controle.\n");
    }
    
    printf("\n---\n");
    printf("Pressione Enter para a próxima rodada...");
    getchar();
    printf("---\n");
}

// --- Função para liberar memória alocada ---
void liberarMemoria(struct Territorio* mapa) {
    free(mapa);
    printf("\nMemória liberada com sucesso!!\n");
}

// --- Função Principal (main) ---
int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    int quantidadeTerritorios;
    struct Territorio* territorios;
    
    printf("===================================\n");
    printf("    SISTEMA DE TERRITÓRIOS WAR\n");
    printf("===================================\n\n");
    
    // Solicita a quantidade de territórios
    printf("Digite a quantidade de territórios: ");
    scanf("%d", &quantidadeTerritorios);
    limparBufferEntrada();
    
    if (quantidadeTerritorios <= 0) {
        printf("Erro: A quantidade de territórios deve ser maior que zero!!\n");
        return 1;
    }
    
    // Aloca memória para os territórios
    territorios = alocarTerritorios(quantidadeTerritorios);
    
    // Cadastra os territórios
    cadastrarTerritorios(territorios, quantidadeTerritorios);
    
    // Exibe o mapa inicial
    exibirMapa(territorios, quantidadeTerritorios);
    
    // --- SISTEMA DE ATAQUES ---
    int opcao;
    do {
        printf("\n=== SISTEMA DE ATAQUES ===\n");
        printf("1 - Realizar ataque\n");
        printf("2 - Exibir mapa atual\n");
        printf("0 - Sair do jogo\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();
        
        switch (opcao) {
            case 1: {
                // Realizar ataque
                int idxAtacante, idxDefensor;
                
                printf("\n--- Seleção do Atacante ---\n");
                printf("Digite o número do território ATACANTE (1 a %d): ", quantidadeTerritorios);
                scanf("%d", &idxAtacante);
                limparBufferEntrada();
                
                printf("\n--- Seleção do Defensor ---\n");
                printf("Digite o número do território DEFENSOR (1 a %d): ", quantidadeTerritorios);
                scanf("%d", &idxDefensor);
                limparBufferEntrada();
                
                // Ajusta índices para base 0
                idxAtacante--;
                idxDefensor--;
                
                // Valida os índices
                if (idxAtacante < 0 || idxAtacante >= quantidadeTerritorios || 
                    idxDefensor < 0 || idxDefensor >= quantidadeTerritorios) {
                    printf("Erro: Índices de território inválidos!!\n");
                    break;
                }
                
                // Valida e executa o ataque
                if (validarAtaque(&territorios[idxAtacante], &territorios[idxDefensor])) {
                    atacar(&territorios[idxAtacante], &territorios[idxDefensor]);
                    // Exibe o mapa atualizado após o ataque
                    exibirMapa(territorios, quantidadeTerritorios);
                }
                break;
            }
            
            case 2:
                // Exibir mapa atual
                exibirMapa(territorios, quantidadeTerritorios);
                break;
                
            case 0:
                printf("Saindo do jogo...\n");
                break;
                
            default:
                printf("Opção inválida!! Tente novamente.\n");
        }
        
    } while (opcao != 0);
    
    // Libera a memória alocada
    liberarMemoria(territorios);
    
    printf("\nJogo encerrado. Até mais!!\n");
    return 0;
}