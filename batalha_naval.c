#include <stdio.h>

#define TAM 10     // Tamanho do tabuleiro 10x10
#define NAVIO 3    // Tamanho fixo dos navios (3 posições)

// Função auxiliar para verificar sobreposição
int verificaSobreposicao(int tabuleiro[TAM][TAM], int linha, int coluna) {
    return tabuleiro[linha][coluna] == 3;
}

int main() {

    /*
        ======================================================
        1. DECLARAÇÃO E INICIALIZAÇÃO DO TABULEIRO
        ======================================================
        Todas as posições começam com 0, representando água.
    */
    int tabuleiro[TAM][TAM];

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            tabuleiro[i][j] = 0;
        }
    }

    /*
        ======================================================
        2. POSIÇÕES DOS 4 NAVIOS
        ======================================================
        — 2 navios normais (horizontal/vertical)
        — 2 navios diagonais
        Todos têm tamanho 3.
    */

    //  Navio 1: Horizontal → começa em (2, 3)
    int h1_linha = 2, h1_coluna = 3;

    //  Navio 2: Vertical → começa em (5, 6)
    int v1_linha = 5, v1_coluna = 6;

    //  Navio 3: Diagonal principal → começa em (1, 1)
    //  Preenche: (1,1), (2,2), (3,3)
    int d1_linha = 1, d1_coluna = 1;

    //  Navio 4: Diagonal secundária → começa em (6, 3)
    //  Preenche: (6,3), (7,2), (8,1)
    int d2_linha = 6, d2_coluna = 3;

    /*
        ======================================================
        3. VALIDAÇÃO DOS LIMITES DO TABULEIRO
        ======================================================
    */

    // Horizontal
    if (h1_coluna + NAVIO > TAM) {
        printf("Erro: Navio horizontal 1 fora do limite.\n");
        return 1;
    }

    // Vertical
    if (v1_linha + NAVIO > TAM) {
        printf("Erro: Navio ve
