#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
#define TAM_NOME 30
#define TAM_COR 10
#define TAM_MISSAO 100
#define TOTAL_MISSOES 5

// --- Definição da Estrutura (Struct) ---
struct Territorio {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
};

// --- Protótipos das Funções ---
void limparBufferEntrada();
struct Territorio* alocarTerritorios(int quantidade);
void cadastrarTerritorios(struct Territorio* territorios, int quantidade, char* corJogador);
void exibirMapa(struct Territorio* territorios, int quantidade, char* corJogador);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void liberarMemoria(struct Territorio* mapa, char* missaoJogador);
int validarAtaque(struct Territorio* atacante, struct Territorio* defensor);
char* atribuirMissao(char* missoes[], int totalMissoes, char* corJogador);
void exibirMissao(char* missao);
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho, char* corJogador);

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
void cadastrarTerritorios(struct Territorio* territorios, int quantidade, char* corJogador) {
    // Cores disponíveis para os territórios inimigos
    char* coresInimigas[] = {"vermelho", "azul", "verde", "amarelo", "roxo", "laranja"};
    int numCoresInimigas = 6;
    
    printf("\n--- Cadastro de %d Territórios ---\n", quantidade);
    
    for (int i = 0; i < quantidade; i++) {
        printf("\n--- Território %d ---\n", i + 1);
        
        // Entrada nome
        printf("Digite o nome do território: ");
        fgets(territorios[i].nome, TAM_NOME, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';
        
        // Define aleatoriamente se o território será do jogador ou inimigo
        // O primeiro território sempre será do jogador para garantir que ele tenha pelo menos um
        if (i == 0) {
            strcpy(territorios[i].cor, corJogador);
            printf("Cor do exército: %s (seu território inicial)\n", corJogador);
        } else {
            // Sorteia uma cor inimiga aleatória
            int corSorteada = rand() % numCoresInimigas;
            strcpy(territorios[i].cor, coresInimigas[corSorteada]);
            printf("Cor do exército: %s (território inimigo)\n", territorios[i].cor);
        }
        
        // Entrada tropas (entre 1 e 5 aleatoriamente)
        territorios[i].tropas = (rand() % 5) + 1;
        printf("Tropas: %d (definidas aleatoriamente)\n", territorios[i].tropas);
    }
}

// --- Função para exibir o mapa ---
void exibirMapa(struct Territorio* territorios, int quantidade, char* corJogador) {
    printf("\n---\n");
    printf("    MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("---\n");
    printf("SUA COR: %s\n\n", corJogador);
    
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(territorios[i].cor, corJogador) == 0) {
            printf("%d. %s (%s, Tropas: %d) ← SEU TERRITÓRIO\n", 
                   i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
        } else {
            printf("%d. %s (%s, Tropas: %d)\n", 
                   i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
        }
    }
    printf("\n");
}

// --- Função para validar se o ataque é permitido ---
int validarAtaque(struct Territorio* atacante, struct Territorio* defensor, char* corJogador) {
    // Verifica se é o mesmo território
    if (atacante == defensor) {
        printf("Erro: Não é possível atacar o próprio território!\n");
        return 0;
    }
    
    // Verifica se o atacante pertence ao jogador
    if (strcmp(atacante->cor, corJogador) != 0) {
        printf("Erro: Você só pode atacar com seus próprios territórios!\n");
        return 0;
    }
    
    // Verifica se as cores são diferentes (não atacar seus próprios territórios)
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
void atacar(struct Territorio* atacante, struct Territorio* defensor, char* corJogador) {
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
        strcpy(defensor->cor, corJogador);
        
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
    printf("Pressione Enter para continuar...");
    getchar();
    printf("---\n");
}

// --- Função para atribuir missão aleatória ao jogador ---
char* atribuirMissao(char* missoes[], int totalMissoes, char* corJogador) {
    char* missao = (char*)malloc(TAM_MISSAO * sizeof(char));
    if (missao == NULL) {
        printf("Erro: Não foi possível alocar memória para a missão!\n");
        exit(1);
    }
    
    // Sorteia uma missão aleatória
    int indiceSorteado = rand() % totalMissoes;
    strcpy(missao, missoes[indiceSorteado]);
    
    return missao;
}

// --- Função para exibir a missão do jogador ---
void exibirMissao(char* missao) {
    printf("\n--- SUA MISSÃO ESTRATÉGICA ---\n");
    printf("%s\n", missao);
    printf("---\n\n");
}

// --- Função para verificar se a missão foi cumprida ---
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho, char* corJogador) {
    // Missão 1: Conquistar 3 territórios seguidos
    if (strstr(missao, "3 territorios seguidos") != NULL) {
        for (int i = 0; i < tamanho - 2; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 &&
                strcmp(mapa[i+1].cor, corJogador) == 0 &&
                strcmp(mapa[i+2].cor, corJogador) == 0) {
                return 1;
            }
        }
    }
    
    // Missão 2: Eliminar todas as tropas da cor vermelha
    else if (strstr(missao, "vermelha") != NULL) {
        int vermelhosEncontrados = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelho") == 0) {
                vermelhosEncontrados = 1;
                break;
            }
        }
        if (!vermelhosEncontrados) return 1;
    }
    
    // Missão 3: Ter pelo menos 10 tropas no total
    else if (strstr(missao, "10 tropas") != NULL) {
        int totalTropas = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                totalTropas += mapa[i].tropas;
            }
        }
        if (totalTropas >= 10) return 1;
    }
    
    // Missão 4: Conquistar todos os territórios de uma cor específica
    else if (strstr(missao, "conquistar todos") != NULL) {
        // Extrai a cor alvo da string da missão
        char corAlvo[TAM_COR];
        if (strstr(missao, "azul") != NULL) {
            strcpy(corAlvo, "azul");
        } else if (strstr(missao, "verde") != NULL) {
            strcpy(corAlvo, "verde");
        } else {
            strcpy(corAlvo, "amarelo"); // fallback
        }
        
        int todosConquistados = 1;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corAlvo) == 0) {
                todosConquistados = 0;
                break;
            }
        }
        if (todosConquistados) return 1;
    }
    
    // Missão 5: Ter o maior exército (mais tropas totais)
    else if (strstr(missao, "maior exercito") != NULL) {
        int tropasJogador = 0;
        int maxOutros = 0;
        
        // Calcula tropas do jogador
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                tropasJogador += mapa[i].tropas;
            }
        }
        
        // Calcula o maior exército inimigo
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) != 0) {
                int tropasEstaCor = 0;
                char corAtual[TAM_COR];
                strcpy(corAtual, mapa[i].cor);
                
                for (int j = i; j < tamanho; j++) {
                    if (strcmp(mapa[j].cor, corAtual) == 0) {
                        tropasEstaCor += mapa[j].tropas;
                        // Marcar para não contar novamente
                        strcpy(mapa[j].cor, "contado");
                    }
                }
                
                if (tropasEstaCor > maxOutros) {
                    maxOutros = tropasEstaCor;
                }
            }
        }
        
        // Restaura as cores originais
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "contado") == 0) {
                // Encontra a cor original olhando os vizinhos
                for (int j = 0; j < tamanho; j++) {
                    if (j != i && strcmp(mapa[j].nome, mapa[i].nome) != 0 && 
                        strcmp(mapa[j].cor, "contado") != 0 && 
                        strcmp(mapa[j].cor, corJogador) != 0) {
                        strcpy(mapa[i].cor, mapa[j].cor);
                        break;
                    }
                }
            }
        }
        
        if (tropasJogador > maxOutros) return 1;
    }
    
    return 0;
}

// --- Função para liberar memória alocada ---
void liberarMemoria(struct Territorio* mapa, char* missaoJogador) {
    free(mapa);
    free(missaoJogador);
    printf("\nMemória liberada com sucesso!!\n");
}

// --- Função Principal (main) ---
int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    int quantidadeTerritorios;
    struct Territorio* territorios;
    char* missaoJogador = NULL;
    char corJogador[TAM_COR];
    
    // Vetor de missões disponíveis
    char* missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Ter pelo menos 10 tropas no total",
        "Conquistar todos os territorios da cor azul",
        "Ter o maior exercito do mundo"
    };
    
    printf("===================================\n");
    printf("    SISTEMA DE TERRITÓRIOS WAR\n");
    printf("===================================\n\n");
    
    // Solicita a cor do jogador
    printf("Digite a cor do seu exército: ");
    fgets(corJogador, TAM_COR, stdin);
    corJogador[strcspn(corJogador, "\n")] = '\0';
    
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
    
    // Cadastra os territórios (agora com a cor do jogador)
    cadastrarTerritorios(territorios, quantidadeTerritorios, corJogador);
    
    // Atribui missão aleatória ao jogador
    missaoJogador = atribuirMissao(missoes, TOTAL_MISSOES, corJogador);
    
    // Exibe a missão do jogador
    exibirMissao(missaoJogador);
    
    // Exibe o mapa inicial
    exibirMapa(territorios, quantidadeTerritorios, corJogador);
    
    // --- SISTEMA DE ATAQUES ---
    int opcao;
    do {
        printf("\n=== SISTEMA DE ATAQUES ===\n");
        printf("1 - Realizar ataque\n");
        printf("2 - Exibir mapa atual\n");
        printf("3 - Ver minha missão\n");
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
                if (validarAtaque(&territorios[idxAtacante], &territorios[idxDefensor], corJogador)) {
                    atacar(&territorios[idxAtacante], &territorios[idxDefensor], corJogador);
                    
                    // Verifica se a missão foi cumprida após o ataque
                    if (verificarMissao(missaoJogador, territorios, quantidadeTerritorios, corJogador)) {
                        printf("\n🎉 PARABÉNS! Você cumpriu sua missão: %s\n", missaoJogador);
                        printf("⭐️ VOCÊ É O VENCEDOR DO JOGO! ⭐️\n\n");
                        
                        liberarMemoria(territorios, missaoJogador);
                        return 0;
                    }
                    
                    // Exibe o mapa atualizado após o ataque
                    exibirMapa(territorios, quantidadeTerritorios, corJogador);
                }
                break;
            }
            
            case 2:
                // Exibir mapa atual
                exibirMapa(territorios, quantidadeTerritorios, corJogador);
                break;
                
            case 3:
                // Exibir missão do jogador
                exibirMissao(missaoJogador);
                break;
                
            case 0:
                printf("Saindo do jogo...\n");
                break;
                
            default:
                printf("Opção inválida!! Tente novamente.\n");
        }
        
    } while (opcao != 0);
    
    // Libera a memória alocada
    liberarMemoria(territorios, missaoJogador);
    
    printf("\nJogo encerrado. Até mais!!\n");
    return 0;
}