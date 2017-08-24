#include "cgi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int determinar_jogador( const dados *dado );

void receber_modo_jogo( const char *string, dados *dado );
void receber_jogada_atual( const char *string, dados *dado );
void receber_jogadas( const char *string, dados *dado );

void receber_dados( const char *data, dados *dado ) {
    unsigned int tamanho;

    if( data != NULL && sscanf( data, "%u", &tamanho ) == 1 ) {
        char *string = calloc( tamanho + 1, sizeof( char ) );
        if( !string ) {
            return;
        }

        fgets( string, tamanho + 1, stdin );

        receber_modo_jogo( string, dado );
        receber_jogada_atual( string, dado );
        receber_jogadas( string, dado );

        dado->atual.jogador = determinar_jogador( dado );

        free( string );
    } else {
        dado->modo_jogo = 0;
        dado->quantidade_jogadas = 0;
        dado->atual.ponto.coluna = 10; // Valor acima do MAX
        dado->atual.ponto.linha = 10;  // Valor acima do MAX
    }
}

void receber_modo_jogo( const char *string, dados *dado ) {
    char *p;
    p = strstr( string, "modo=" );
    sscanf( p, "modo=%u%*s", &dado->modo_jogo );
}

void receber_jogada_atual( const char *string, dados *dado ) {
    char *p;
    p = strstr( string, "x=" );
    if( p ) {
        if( !sscanf( p, "x=%u&y=%u", &dado->atual.ponto.coluna, &dado->atual.ponto.linha ) ) {
            dado->atual.ponto.coluna = 10; // Valor acima do MAX
            dado->atual.ponto.linha = 10;  // Valor acima do MAX
            return;
        }
        dado->atual.ponto.coluna -= 1;
        dado->atual.ponto.linha -= 1;
    }
}

void receber_jogadas( const char *string, dados *dado ) {
    char *p;
    p = strstr( string, "jogadas=" );
    p += strlen( "jogadas=" );
    if( *( p + 1 ) != '\0' ) {
        int i = 0;

        while( sscanf( p,
                       "%u_%u_%u-",
                       &dado->jogadas[i].jogador,
                       &dado->jogadas[i].ponto.linha,
                       &dado->jogadas[i].ponto.coluna ) > 0 ) {
            i++;
            p = strstr( p, "-" ) + 1;
        }
        dado->quantidade_jogadas = i;
    } else {
        dado->jogadas[0].jogador = -1;
        dado->quantidade_jogadas = 0;
    }
}

unsigned int determinar_jogador( const dados *dado ) {
    if( dado->quantidade_jogadas == 0 ||
        dado->jogadas[dado->quantidade_jogadas - 1].jogador % 2 == 0 ) {
        return P1;
    } else {
        return P2;
    }
}
