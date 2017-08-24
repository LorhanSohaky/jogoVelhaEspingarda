#ifndef PRINTS_H
#define PRINTS_H

typedef struct {
    unsigned int linha;
    unsigned int coluna;
} coordenadas;

typedef struct {
    unsigned int jogador;
    coordenadas ponto;
} jogada;

typedef struct {
    unsigned int modo_jogo;
    unsigned int quantidade_jogadas;
    jogada jogadas[MAX];
    jogada atual;
} dados;

typedef struct {
    unsigned int *matriz;
    unsigned int ordem_matriz;
} Tabuleiro;

#endif
