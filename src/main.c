#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define max 9
#define lin 3
#define col 3

enum { P1 = 1, P2 = 2 };

typedef struct {
    unsigned int linha;
    unsigned int coluna;
} coordenadas;

typedef struct {
    unsigned int jogador;
    coordenadas ponto;
} jogada;

bool esta_vazio( int *tabuleiro, coordenadas posicao );

void print_tabuleiro( int *tabuleiro ) {
    printf( "<p> %d | %d | %d </p>", tabuleiro[0], tabuleiro[1], tabuleiro[2] );
    printf( "<p> -------------</p>" );
    printf( "<p> %d | %d | %d </p>", tabuleiro[3], tabuleiro[4], tabuleiro[5] );
    printf( "<p> -------------</p>" );
    printf( "<p> %d | %d | %d </p>", tabuleiro[6], tabuleiro[7], tabuleiro[8] );
}

void print_jogo( int *tabuleiro ) {
    print_tabuleiro( tabuleiro );
}

void print_form( int *tabuleiro ) {
    printf( "<form method=\"POST\" action=\"main\">" );
    printf( "Coordenada x: <input type=\"number\" name=\"x\"><br>" );
    printf( "Coordenada y: <input type=\"number\" name=\"y\"><br>" );
    printf( "<input type=\"submit\" value=\"Enviar\">" );

    printf( "<input type=\"hidden\" name=\"jogadas\" value=\"" );
    int i, j;
    for( i = 0; i < lin; i++ ) {
        for( j = 0; j < col; j++ ) {
            printf( "%d_%d_%d&", tabuleiro[lin * i + j], i, j );
        }
    }
    printf( "\">" );
}

int main() {
    char *data;
    char vet[300];

    unsigned int tamanho;

    int *tabuleiro = calloc( lin * col, sizeof( int ) );
    printf( "%s%c%c\n", "Content-Type:text/html;charset=utf-8", 13, 10 );
    printf( "<TITLE>Jogo da velha</TITLE>\n" );

    data = getenv( "QUERY_STRING" );
    sscanf( data, "%d", &tamanho );
    fgets( vet, tamanho + 1, stdin );

    printf( "<p>Stdin:%s</p>\n", vet );

    if( data ) {
        printf( "<p>veio</p>" );
        printf( "<p>%s</p>", data );
    }

    coordenadas posicao;
    posicao.linha = 1;
    posicao.coluna = 0;
    printf( "<p>%d</p>\n", esta_vazio( tabuleiro, posicao ) );
    int k = 1;
    for( int i = 0; i < 3; i++ ) {
        for( int j = 0; j < 3; j++ ) {
            tabuleiro[3 * i + j] = 0;
        }
    }

    print_jogo( tabuleiro );
    print_form( tabuleiro );

    return 0;
}

void ler_modo_jogo() {
}

bool esta_vazio( int *tabuleiro, coordenadas posicao ) {
    return tabuleiro[3 * posicao.linha + posicao.coluna];
}

bool init( int *tabuleiro ) {
    tabuleiro = calloc( max, sizeof( int ) );

    return tabuleiro;
}
