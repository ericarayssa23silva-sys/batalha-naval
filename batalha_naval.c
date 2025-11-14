#include <stdio.h>

#define TAM 10      // Tamanho do tabuleiro (10x10)
#define NAVIO 3     // Tamanho fixo dos navios

int main() {

    /*
        ======================================================
        1. DECLARAÇÃO DO TABULEIRO E INICIALIZAÇÃO COM ÁGUA
        ======================================================
        O tabuleiro é uma matriz 10x10 onde:
            0 = água
            3 = parte do navio
    */

    int tabuleiro[TAM][TAM];

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            tabuleiro[i][j] = 0;  // Preenche com água
        }
    }

    /*
        ======================================================
        2. CRIAÇÃO DOS NAVIOS (VETORES DE TAMANHO 3)
        ======================================================
        Cada navio é representado por 3 valores iguais a 3.
    */

    int navioHorizontal[NAVIO] = {3, 3, 3};
    int navioVertical[NAVIO] = {3, 3, 3};

    /*
        ======================================================
        3. COORDENADAS INICIAIS DOS NAVIOS
        ======================================================
        Como o exercício permite definir no código, escolhemos:
        
        - Navio horizontal começando na posição (2,4)
        - Navio vertical começando na posição (5,1)
        
        OBS: As posições respeitam os limites e não se sobrepõem.
    */

    int linhaH = 2, colunaH = 4;
    int linhaV = 5, colunaV = 1;

    /*
        ======================================================
        4. VALIDAÇÃO SIMPLES DE LIMITES DO TABULEIRO
        ======================================================
        Garante que os navios de tamanho 3 não ultrapassem o vetor.
    */

    if (colunaH + NAVIO > TAM) {
        printf("Erro: Navio horizontal está fora dos limites do tabuleiro.\n");
        return 1;
    }

    if (linhaV + NAVIO > TAM) {
        printf("Erro: Navio vertical está fora dos limites do tabuleiro.\n");
        return 1;
    }

    /*
        ======================================================
        5. VALIDAÇÃO DE SOBREPOSIÇÃO DOS NAVIOS
        ======================================================
        Antes de posicionar, verifica se o local já está ocupado.
    */

    for (int i = 0; i < NAVIO; i++) {

        // Verifica posição do navio horizontal
        if (tabuleiro[linhaH][colunaH + i] == 3) {
            printf("Erro: Navio horizontal sobreposto!\n");
            return 1;
        }

        // Verifica posição do navio vertical
        if (tabuleiro[linhaV + i][colunaV] == 3) {
            printf("Erro: Navio vertical sobreposto!\n");
            return 1;
        }
    }

    /*
        ======================================================
        6. POSICIONAMENTO DOS NAVIOS NA MATRIZ
        ======================================================
    */

    // Posiciona navio horizontal
    for (int i = 0; i < NAVIO; i++) {
        tabuleiro[linhaH][colunaH + i] = navioHorizontal[i];
    }

    // Posiciona navio vertical
    for (int i = 0; i < NAVIO; i++) {
        tabuleiro[linhaV + i][colunaV] = navioVertical[i];
    }

    /*
        ======================================================
        7. EXIBIÇÃO DO TABULEIRO COMPLETO
        ======================================================
        Imprime a matriz usando loops aninhados.
    */

    printf("\n===== TABULEIRO DE BATALHA NAVAL =====\n\n");

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            printf("%d ", tabuleiro[i][j]);
        }
        printf("\n");
    }

    return 0;
}
