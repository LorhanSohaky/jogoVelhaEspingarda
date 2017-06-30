#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { unsigned int linha, coluna; } coordenadas;

bool esta_vazio( int *tabuleiro, coordenadas posicao );

void print_jogo( int *tabuleiro ) {
    printf( "<p> %d | %d | %d </p>", tabuleiro[0], tabuleiro[1], tabuleiro[2] );
    printf( "<p> -------------</p>" );
    printf( "<p> %d | %d | %d </p>", tabuleiro[3], tabuleiro[4], tabuleiro[5] );
    printf( "<p> -------------</p>" );
    printf( "<p> %d | %d | %d </p>", tabuleiro[6], tabuleiro[7], tabuleiro[8] );
}

void print_widget( int *tabuleiro ) {
    printf( "<input type=\"text\" name=\"teste\" value=\"" );
    int i, j;
    for( i = 0; i < 3; i++ ) {
        for( j = 0; j < 3; j++ ) {
            printf( "%d %d %d;", tabuleiro[3 * i + j], i, j );
        }
    }

    printf( "\">" );
}

int main() {
    char *data;
    int *tabuleiro = calloc( 9, sizeof( int ) );
    float temperatura;
    printf( "%s%c%c\n", "Content-Type:text/html;charset=utf-8", 13, 10 );
    printf( "<TITLE>Jogo da velha</TITLE>\n" );

    data = getenv( "QUERY_STRING" );

    if( sscanf( data, "temperatura=%f", &temperatura ) != 1 ) {
        printf( "<P>Error! Invalid data. Data must be numeric." );
    } else {
        printf( "<P>Temperatura em Celsius %f para Fahrenheit %f</P>",
                temperatura,
                temperatura * 18000 + 32.00 );
    }

    coordenadas posicao;
    posicao.linha = 1;
    posicao.coluna = 0;
    printf( "<p>%d</p>\n", esta_vazio( tabuleiro, posicao ) );
    int k = 1;
    for( int i = 0; i < 3; i++ ) {
        for( int j = 0; j < 3; j++ ) {
            tabuleiro[3 * i + j] = k++;
        }
    }

    print_widget( tabuleiro );
    print_jogo( tabuleiro );

    return 0;
}

bool esta_vazio( int *tabuleiro, coordenadas posicao ) {
    return tabuleiro[3 * posicao.linha + posicao.coluna];
}
