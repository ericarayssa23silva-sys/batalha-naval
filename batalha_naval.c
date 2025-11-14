#include <stdio.h>

#define TAM 10     // tamanho do tabuleiro (10x10)
#define NAVIO 3    // tamanho fixo dos navios
#define SKILL_SIZE 5 // dimensão das matrizes de habilidade (5x5)
                 // usamos SKILL_SIZE ímpar para poder ter um "centro" claro

// Função auxiliar: inicializa tabuleiro com 0
void inicializaTabuleiro(int tab[TAM][TAM]) {
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            tab[i][j] = 0;
}

// Função auxiliar: checa sobreposição simples (retorna 1 se ocupado por navio)
int ocupadoPorNavio(int tab[TAM][TAM], int linha, int coluna) {
    return tab[linha][coluna] == 3;
}

// Imprime o tabuleiro com legenda: 0 = água, 3 = navio, 5 = área de habilidade
void imprimeTabuleiro(int tab[TAM][TAM]) {
    printf("\nLegenda: 0 = água | 3 = navio | 5 = área de habilidade\n\n");
    printf("   ");
    for (int c = 0; c < TAM; c++) printf("%d ", c); // cabeçalho de colunas
    printf("\n");
    for (int i = 0; i < TAM; i++) {
        printf("%2d ", i); // índice da linha
        for (int j = 0; j < TAM; j++) {
            printf("%d ", tab[i][j]);
        }
        printf("\n");
    }
}

// Constrói dinamicamente a matriz do CONE (apontando para baixo)
// Regra utilizada: para as linhas r = 0..2 (topo -> base), col é afetado se abs(col - center) <= r
// Dimensão = SKILL_SIZE (5), mas cone terá altura efetiva 3 no topo da matriz (linhas 0..2)
void constroiCone(int cone[SKILL_SIZE][SKILL_SIZE]) {
    int center = SKILL_SIZE / 2; // 2 para SKILL_SIZE = 5
    for (int r = 0; r < SKILL_SIZE; r++) {
        for (int c = 0; c < SKILL_SIZE; c++) {
            // apenas as primeiras 3 linhas (0,1,2) formam o cone; as linhas 3 e 4 ficam vazias
            if (r <= 2 && ( (c - center) < = r) && ((center - c) <= r) ) {
                // simplificação: equivalência a abs(c - center) <= r
                // C não permite abs macro sem stdlib; fazer explicitamente:
                if ((c - center) < 0 ? (center - c) <= r : (c - center) <= r)
                    cone[r][c] = 1;
                else
                    cone[r][c] = 0;
            } else {
                cone[r][c] = 0;
            }
        }
    }
}

// Constrói dinamicamente a matriz da CRUZ (centro linha e coluna)
// Regra: posição é 1 se r == center ou c == center
void constroiCruz(int cruz[SKILL_SIZE][SKILL_SIZE]) {
    int center = SKILL_SIZE / 2;
    for (int r = 0; r < SKILL_SIZE; r++) {
        for (int c = 0; c < SKILL_SIZE; c++) {
            if (r == center || c == center) cruz[r][c] = 1;
            else cruz[r][c] = 0;
        }
    }
}

// Constrói dinamicamente a matriz do "Octaedro" (vista frontal -> losango/diamante)
// Usamos condição de Manhattan distance: abs(r-center) + abs(c-center) <= center
void constroiOctaedro(int oct[SKILL_SIZE][SKILL_SIZE]) {
    int center = SKILL_SIZE / 2;
    for (int r = 0; r < SKILL_SIZE; r++) {
        for (int c = 0; c < SKILL_SIZE; c++) {
            int dr = r - center; if (dr < 0) dr = -dr;
            int dc = c - center; if (dc < 0) dc = -dc;
            if (dr + dc <= center) oct[r][c] = 1;
            else oct[r][c] = 0;
        }
    }
}

// Sobrepõe a matriz de habilidade (skillMat) ao tabuleiro tab, centrada em (origLinha, origColuna)
// Regras:
//  - skillMat tem dimensão SKILL_SIZE x SKILL_SIZE
//  - centro da skillMat é (SKILL_SIZE/2, SKILL_SIZE/2)
//  - somente marca com 5 as células do tab que NÃO já são navio (3)
//  - respeita limites do tabuleiro
void aplicarSkillNoTabuleiro(int tab[TAM][TAM], int skillMat[SKILL_SIZE][SKILL_SIZE],
                             int origLinha, int origColuna) {
    int center = SKILL_SIZE / 2;
    for (int r = 0; r < SKILL_SIZE; r++) {
        for (int c = 0; c < SKILL_SIZE; c++) {
            if (skillMat[r][c] == 1) {
                int boardR = origLinha - center + r;
                int boardC = origColuna - center + c;
                // valida limite do tabuleiro
                if (boardR >= 0 && boardR < TAM && boardC >= 0 && boardC < TAM) {
                    // só marca área afetada se não houver navio (mantemos navio = 3)
                    if (tab[boardR][boardC] == 0) {
                        tab[boardR][boardC] = 5; // 5 = área afetada por habilidade
                    }
                    // se já for 3, deixamos 3 (navio prevalece visualmente)
                }
            }
        }
    }
}

int main() {
    int tabuleiro[TAM][TAM];
    inicializaTabuleiro(tabuleiro);

    // -------------------------------------------------------
    // Posicionamento dos 4 navios (tamanho NAVIO = 3) - definidos no código
    // -------------------------------------------------------
    // Navio 1: Horizontal, começa em (2,3)
    int h1_l = 2, h1_c = 3;
    // Navio 2: Vertical, começa em (5,6)
    int v1_l = 5, v1_c = 6;
    // Navio 3: Diagonal principal, começa em (1,1) -> (1,1),(2,2),(3,3)
    int d1_l = 1, d1_c = 1;
    // Navio 4: Diagonal secundária, começa em (6,3) -> (6,3),(7,2),(8,1)
    int d2_l = 6, d2_c = 3;

    // Valida limites simples e posiciona (assumimos configuração válida; checamos só para segurança)
    if (h1_c + NAVIO > TAM || v1_l + NAVIO > TAM ||
        d1_l + NAVIO > TAM || d1_c + NAVIO > TAM ||
        d2_l + NAVIO > TAM || d2_c - (NAVIO - 1) < 0) {
        printf("Erro: coordenas de navio fora do limite. Verifique inicializadores.\n");
        return 1;
    }

    // Checagem de sobreposição simplificada e posicionamento
    for (int i = 0; i < NAVIO; i++) {
        // horizontal
        if (ocupadoPorNavio(tabuleiro, h1_l, h1_c + i)) {
            printf("Erro: sobreposição ao posicionar navio horizontal.\n");
            return 1;
        }
        tabuleiro[h1_l][h1_c + i] = 3;

        // vertical
        if (ocupadoPorNavio(tabuleiro, v1_l + i, v1_c)) {
            printf("Erro: sobreposição ao posicionar navio vertical.\n");
            return 1;
        }
        tabuleiro[v1_l + i][v1_c] = 3;

        // diagonal principal
        if (ocupadoPorNavio(tabuleiro, d1_l + i, d1_c + i)) {
            printf("Erro: sobreposição ao posicionar navio diagonal principal.\n");
            return 1;
        }
        tabuleiro[d1_l + i][d1_c + i] = 3;

        // diagonal secundária
        if (ocupadoPorNavio(tabuleiro, d2_l + i, d2_c - i)) {
            printf("Erro: sobreposição ao posicionar navio diagonal secundária.\n");
            return 1;
        }
        tabuleiro[d2_l + i][d2_c - i] = 3;
    }

    // -------------------------------------------------------
    // Construção dinâmica das matrizes de habilidade (5x5)
    // -------------------------------------------------------
    int cone[SKILL_SIZE][SKILL_SIZE];
    int cruz[SKILL_SIZE][SKILL_SIZE];
    int octaedro[SKILL_SIZE][SKILL_SIZE];

    constroiCone(cone);
    constroiCruz(cruz);
    constroiOctaedro(octaedro);

    // (Opcional) -- imprimir as matrizes de habilidade no console para verificar
    printf("Matriz - CONE (1 = afetado):\n");
    for (int r = 0; r < SKILL_SIZE; r++) {
        for (int c = 0; c < SKILL_SIZE; c++) printf("%d ", cone[r][c]);
        printf("\n");
    }
    printf("\nMatriz - CRUZ (1 = afetado):\n");
    for (int r = 0; r < SKILL_SIZE; r++) {
        for (int c = 0; c < SKILL_SIZE; c++) printf("%d ", cruz[r][c]);
        printf("\n");
    }
    printf("\nMatriz - OCTAEDRO/LOSANGO (1 = afetado):\n");
    for (int r = 0; r < SKILL_SIZE; r++) {
        for (int c = 0; c < SKILL_SIZE; c++) printf("%d ", octaedro[r][c]);
        printf("\n");
    }
    printf("\n");

    // -------------------------------------------------------
    // Definir pontos de origem no tabuleiro para as habilidades
    // (linhas/colunas definidas diretamente no código - ajustar conforme desejar)
    // -------------------------------------------------------
    int origemConeLinha = 0, origemConeColuna = 4;   // exemplo: topo região (cone "aponta para baixo")
    int origemCruzLinha = 4, origemCruzColuna = 2;   // exemplo: centro-esquerda
    int origemOctLinha = 7, origemOctColuna = 7;     // exemplo: canto inferior direito

    // -------------------------------------------------------
    // Aplicar habilidades (sobreposição com checagem de limites)
    // -------------------------------------------------------
    aplicarSkillNoTabuleiro(tabuleiro, cone, origemConeLinha, origemConeColuna);
    aplicarSkillNoTabuleiro(tabuleiro, cruz, origemCruzLinha, origemCruzColuna);
    aplicarSkillNoTabuleiro(tabuleiro, octaedro, origemOctLinha, origemOctColuna);

    // -------------------------------------------------------
    // Exibir tabuleiro final com navios (3) e áreas de habilidade (5)
    // -------------------------------------------------------
    imprimeTabuleiro(tabuleiro);

    return 0;
}
