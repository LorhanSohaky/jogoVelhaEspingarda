#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIN 3
#define COL 3
#define MAX ( LIN ) * ( COL )

enum { P1 = 1, P2 = 2 };
enum { PVP = 1, PC = 2 };

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
    jogada jogadas[MAX];
    jogada atual;
} dados;

bool esta_vazio( int *tabuleiro, coordenadas posicao );
void print_tabuleiro( int *tabuleiro );
void print_jogo( int *tabuleiro );
void print_form( int *tabuleiro );

int main() {
    char *data;
    char *string;

    int *tabuleiro = calloc( MAX, sizeof( int ) );
    unsigned int tamanho;

    printf( "%s%c%c\n", "Content-Type:text/html;charset=utf-8", 13, 10 );
    printf( "<TITLE>Jogo da velha</TITLE>\n" );

    data = getenv( "CONTENT_LENGTH" );
    if( data != NULL && sscanf( data, "%d", &tamanho ) == 1 ) {
        string = calloc( tamanho + 1, sizeof( char ) );
        if( !string ) {
            free( tabuleiro );
            return 1;
        }

        fgets( string, tamanho + 1, stdin );

        printf( "<p>Stdin:%s</p>\n", string );
    }

    print_jogo( tabuleiro );

    free( tabuleiro );
    free( string );

    return 0;
}

dados receber_dados( char *data ) {
    dados post;
    return post;
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
