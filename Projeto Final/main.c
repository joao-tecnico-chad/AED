#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX 100

typedef struct {
    int linhas;
    int colunas;
    int variante;
    int matriz[MAX][MAX];
} Parede;

int matrizOriginal[MAX][MAX]; // Variável global para armazenar a matriz original

bool isValid(int x, int y, int linhas, int colunas) {
    return (x >= 0 && x < linhas && y >= 0 && y < colunas);
}

int dfs(Parede *parede, int x, int y, int cor, bool visited[MAX][MAX]) {
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    visited[x][y] = true;
    int tamanho = 1;

    for (int i = 0; i < 4; i++) {
        int newX = x + dx[i];
        int newY = y + dy[i];

        if (isValid(newX, newY, parede->linhas, parede->colunas) && 
            !visited[newX][newY] && 
            parede->matriz[newX][newY] == cor) {
            tamanho += dfs(parede, newX, newY, cor, visited);
        }
    }

    return tamanho;
}

void removerMancha(Parede *parede, int x, int y, int cor, bool visited[MAX][MAX]) {
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    visited[x][y] = true;
    parede->matriz[x][y] = -1;

    for (int i = 0; i < 4; i++) {
        int newX = x + dx[i];
        int newY = y + dy[i];

        if (isValid(newX, newY, parede->linhas, parede->colunas) && 
            !visited[newX][newY] && 
            parede->matriz[newX][newY] == cor) {
            removerMancha(parede, newX, newY, cor, visited);
        }
    }
}

void aplicarGravidadeVertical(Parede *parede) {
    for (int j = 0; j < parede->colunas; j++) {
        int vazios = 0;
        for (int i = 0; i < parede->linhas; i++) {
            if (parede->matriz[i][j] == -1) {
                vazios++;
            } else if (vazios > 0) {
                parede->matriz[i - vazios][j] = parede->matriz[i][j];
                parede->matriz[i][j] = -1;
            }
        }
    }
}

void aplicarGravidadeHorizontal(Parede *parede) {
    int vazias = 0;
    for (int j = parede->colunas - 1; j >= 0; j--) {
        bool colunaVazia = true;
        for (int i = 0; i < parede->linhas; i++) {
            if (parede->matriz[i][j] != -1) {
                colunaVazia = false;
                break;
            }
        }

        if (colunaVazia) {
            vazias++;
        } else if (vazias > 0) {
            for (int i = 0; i < parede->linhas; i++) {
                parede->matriz[i][j + vazias] = parede->matriz[i][j];
                parede->matriz[i][j] = -1;
            }
        }
    }
}

void imprimirParede(Parede *parede) {
    for (int i = parede->linhas - 1; i >= 0; i--) {
        for (int j = 0; j < parede->colunas; j++) {
            printf("%d ", parede->matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void copiarMatriz(int dest[MAX][MAX], int src[MAX][MAX], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

// Função para verificar se uma posição já foi tentada
bool verificarPosicaoTentada(int x, int y) {
    return matrizOriginal[x][y] == -2;
}

// Função para marcar uma posição como tentada
void marcarPosicaoTentada(int x, int y) {
    matrizOriginal[x][y] = -2;
}

typedef struct {
    int x;
    int y;
} Jogada;

int resolverParede(Parede *parede, FILE *saida) {
    bool visited[MAX][MAX] = {false};
    int jogadas = 0;
    int pontos = 0;
    Jogada listaJogadas[MAX * MAX]; // Lista para armazenar as jogadas

    // Função recursiva de backtracking
    bool backtrack(int x, int y) {
        if (x == parede->linhas) {
            x = 0;
            y++;
            if (y == parede->colunas) {
                return true; // Todas as posições foram tentadas
            }
        }

        if (parede->matriz[x][y] == -1 || verificarPosicaoTentada(x, y)) {
            return backtrack(x + 1, y); // Vá para a próxima posição
        }

        int corAtual = parede->matriz[x][y];
        int tamanho = dfs(parede, x, y, corAtual, visited);
        if (tamanho > 1) {
            marcarPosicaoTentada(x, y);
            listaJogadas[jogadas].x = x + 1;
            listaJogadas[jogadas].y = y + 1;
            jogadas++;
            pontos += tamanho * (tamanho - 1);
            memset(visited, false, sizeof(visited));
            removerMancha(parede, x, y, corAtual, visited);
            aplicarGravidadeVertical(parede);
            aplicarGravidadeHorizontal(parede);
            if (backtrack(0, 0)) {
                return true;
            }
            // Desfazer a jogada
            jogadas--;
            pontos -= tamanho * (tamanho - 1);
            copiarMatriz(parede->matriz, matrizOriginal, parede->linhas, parede->colunas);
        }

        return backtrack(x + 1, y);
    }

    copiarMatriz(matrizOriginal, parede->matriz, parede->linhas, parede->colunas);
    backtrack(0, 0);

    // Escreva o cabeçalho e as jogadas no arquivo de saída
    fprintf(saida, "%d %d %d\n%d %d\n", parede->linhas, parede->colunas, parede->variante, jogadas, pontos);
    for (int i = 0; i < jogadas; i++) {
        fprintf(saida, "%d %d\n", listaJogadas[i].x, listaJogadas[i].y);
    }

    printf("Total de jogadas: %d\n", jogadas);
    printf("Pontuação total: %d\n", pontos);

    return pontos;
}

Parede *lerParede(char *nomeFicheiro) {
    FILE *f = fopen(nomeFicheiro, "r");
    if (!f) {
        perror("Erro ao abrir o ficheiro para leitura");
        return NULL;
    }

    Parede *parede = malloc(sizeof(Parede));
    fscanf(f, "%d %d %d", &parede->linhas, &parede->colunas, &parede->variante);

    for (int i = parede->linhas - 1; i >= 0; i--) {
        for (int j = 0; j < parede->colunas; j++) {
            fscanf(f, "%d", &parede->matriz[i][j]);
        }
    }

    fclose(f);
    return parede;
}

void liberarMemoria(Parede *parede) {
    free(parede);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <nome do ficheiro de entrada>\n", argv[0]);
        return 1;
    }

    Parede *parede = lerParede(argv[1]);
    if (!parede) {
        printf("Erro ao ler o ficheiro de entrada.\n");
        return 1;
    }

    imprimirParede(parede);
    char nomeFicheiroSaida[256];
    snprintf(nomeFicheiroSaida, sizeof(nomeFicheiroSaida), "%s.tileblasts", argv[1]);
    FILE *saida = fopen(nomeFicheiroSaida, "w+");

    int maxPontos = 0;
    for (int corTentativa = 0; corTentativa < 10; corTentativa++) {
        copiarMatriz(parede->matriz, matrizOriginal, parede->linhas, parede->colunas);
        int pontos = resolverParede(parede, saida);
        if (pontos > maxPontos) {
            maxPontos = pontos;
        }
        if (maxPontos >= parede->variante) {
            break;
        }
    }

    if (maxPontos < parede->variante) {
        printf("Não foi possível alcançar a pontuação mínima desejada.\n");
    }

    fclose(saida);
    liberarMemoria(parede);

    return 0;
}
