#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 9
#define LIN 3
#define COL 3

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
void print_tabuleiro( int *tabuleiro );
void print_jogo( int *tabuleiro );
void print_form( int *tabuleiro );

int main() {
    char *data;
    int c[9];
    char vet[300];
    unsigned int tamanho;

    int *tabuleiro = c;
    printf( "%s%c%c\n", "Content-Type:text/html;charset=utf-8", 13, 10 );
    printf( "<TITLE>Jogo da velha</TITLE>\n" );

    data = getenv( "QUERY_STRING" );
    printf( "<p>Pointer:%p</p>", data );
    if( data != NULL || sscanf( data, "%d", &tamanho ) == 1 ) {
        sscanf( data, "%d", &tamanho );
        fgets( vet, tamanho + 1, stdin );
        printf( "<p>Stdin:%s</p>\n", vet );
    }

    print_jogo( tabuleiro );

    return 0;
}

void print_tabuleiro( int *tabuleiro ) {
    printf( "<p> %d | %d | %d </p>", tabuleiro[0], tabuleiro[1], tabuleiro[2] );
    printf( "<p> -------------</p>" );
    printf( "<p> %d | %d | %d </p>", tabuleiro[3], tabuleiro[4], tabuleiro[5] );
    printf( "<p> -------------</p>" );
    printf( "<p> %d | %d | %d </p>", tabuleiro[6], tabuleiro[7], tabuleiro[8] );
}

void print_jogo( int *tabuleiro ) {
    print_tabuleiro( tabuleiro );
    print_form( tabuleiro );
}

void print_form( int *tabuleiro ) {
    printf( "<form method=\"POST\" action=\"main\">" );
    printf( "Coordenada x: <input type=\"number\" name=\"x\"><br>" );
    printf( "Coordenada y: <input type=\"number\" name=\"y\"><br>" );
    printf( "<input type=\"submit\" value=\"Enviar\">" );

    printf( "<input type=\"hidden\" name=\"jogadas\" value=\"" );
    int i, j;
    for( i = 0; i < LIN; i++ ) {
        for( j = 0; j < COL; j++ ) {
            printf( "%d_%d_%d-", tabuleiro[LIN * i + j], i, j );
        }
    }
    printf( "\">" );
}

void ler_modo_jogo() {
}

bool esta_vazio( int *tabuleiro, coordenadas posicao ) {
    return tabuleiro[3 * posicao.linha + posicao.coluna];
}

bool init( int *tabuleiro ) {
    tabuleiro = calloc( MAX, sizeof( int ) );

    return tabuleiro;
}
