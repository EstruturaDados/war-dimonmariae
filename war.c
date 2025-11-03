#include <stdio.h>
#include <string.h>

// --- Constantes Globais ---
#define MAX_TERRITORIOS 5
#define TAM_NOME 30
#define TAM_COR 10

// --- Definição da Estrutura (Struct) ---
struct Territorio {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
};

// --- Função para limpar o buffer de entrada ---
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Função Principal (main) ---
int main() {
    // Declaração do vetor de structs
    struct Territorio territorios[MAX_TERRITORIOS];
    int i;
    
    printf("===================================\n");
    printf("    SISTEMA DE TERRITÓRIOS\n");
    printf("===================================\n\n");

    // --- CADASTRO DOS TERRITÓRIOS ---
    printf("--- Cadastro de 5 Territórios ---\n");
    for (i = 0; i < MAX_TERRITORIOS; i++) {
        printf("\n--- Território %d ---\n", i + 1);
        
        // Entrada nome
        printf("Digite o nome do território: ");
        fgets(territorios[i].nome, TAM_NOME, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0'; // Remove o \n
        
        // Entrada cor
        printf("Digite a cor do exército: ");
        fgets(territorios[i].cor, TAM_COR, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0'; // Remove o \n
        
        // Entrada tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
        limparBufferEntrada(); // Limpa o buffer após scanf
    }

    // --- EXIBIÇÃO DO MAPA DO MUNDO ---
    printf("\n\n============================================\n");
    printf("      MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("============================================\n");
    
    for (i = 0; i < MAX_TERRITORIOS; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("  Nome: %s\n", territorios[i].nome);
        printf("  Cor do exército: %s\n", territorios[i].cor);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("  --------------------\n");
    }
    
    printf("\nTotal de territórios controlados: %d\n", MAX_TERRITORIOS);
    printf("============================================\n");

    return 0;
}