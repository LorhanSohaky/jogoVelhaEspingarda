#ifndef DATATYPE_H
#define DATATYPE_H

#define P1 1
#define P2 2

#define PVP 1
#define PC 2

#define LIN 3
#define COL 3
#define MAX ( LIN ) * ( COL )

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

#endif
