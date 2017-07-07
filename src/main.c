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
    unsigned int quantidade_jogadas;
    jogada jogadas[MAX];
    jogada atual;
} dados;

bool esta_vazio( int *tabuleiro, coordenadas posicao );
unsigned int determinar_jogador( dados *dado );

void print_tabuleiro( int *tabuleiro );
void print_jogo( dados *dado, int *tabuleiro );
void print_form( dados *dado );

void receber_dados( char *data, dados *dado );
void receber_modo_jogo( char *string, dados *dado );
void receber_jogada_atual( char *string, dados *dado );
void receber_jogadas( char *string, dados *dado );

void add_jogadas_ao_tabuleiro( jogada jogo, int *tabuleiro );
void add_atual_as_jogadas( dados *dado );

int main() {
    dados dado;
    char *data;
    int *tabuleiro = calloc( MAX, sizeof( int ) );

    printf( "%s%c%c\n", "Content-Type:text/html;charset=utf-8", 13, 10 );
    printf( "<TITLE>Jogo da velha</TITLE>\n" );

    data = getenv( "CONTENT_LENGTH" );
    receber_dados( data, &dado );

    /*if( esta_vazio( tabuleiro, dado.atual.ponto ) ) {
        add_jogadas_ao_tabuleiro( dado.atual, tabuleiro );
        add_atual_as_jogadas( &dado );
    }*/

    print_jogo( &dado, tabuleiro );

    free( tabuleiro );

    return 0;
}

void receber_dados( char *data, dados *dado ) {
    unsigned int tamanho;
    char *string;
    int i;

    if( data != NULL && sscanf( data, "%d", &tamanho ) == 1 ) {
        string = calloc( tamanho + 1, sizeof( char ) );
        if( !string ) {
            return;
        }

        fgets( string, tamanho + 1, stdin );

        printf( "<p>Stdin:%s</p>\n", string );

        receber_modo_jogo( string, dado );
        receber_jogada_atual( string, dado );
        receber_jogadas( string, dado );
        dado->atual.jogador = determinar_jogador( dado );

        printf( "MODO:%d\n", dado->modo_jogo );
        printf( "JOGADA ATUAL:linha=%d coluna=%d\n",
                dado->atual.ponto.linha,
                dado->atual.ponto.coluna );

        for( i = 0; i < MAX; i++ ) {
            printf( "<p>Jogador: %d</p>", dado->jogadas[i].jogador );
            printf( "<p>Linha: %d  Coluna: %d</p><br>",
                    dado->jogadas[i].ponto.linha,
                    dado->jogadas[i].ponto.coluna );
        }

        free( string );
    }
}

unsigned int determinar_jogador( dados *dado ) {
    if( dado->jogadas[dado->quantidade_jogadas - 1].jogador % 2 == 0 ) {
        return P1;
    } else {
        return P2;
    }
}

void add_jogadas_ao_tabuleiro( jogada jogo, int *tabuleiro ) {
    tabuleiro[jogo.ponto.linha + jogo.ponto.coluna] = jogo.jogador;
}

void add_atual_as_jogadas( dados *dado ) {
    dado->jogadas[dado->quantidade_jogadas] = dado->atual;
    dado->quantidade_jogadas++;
}

void receber_modo_jogo( char *string, dados *dado ) {
    char *p;
    p = strstr( string, "modo=" );
    sscanf( p, "modo=%d%*s", &dado->modo_jogo );
}

void receber_jogada_atual( char *string, dados *dado ) {
    char *p;
    p = strstr( string, "x=" );
    sscanf( p, "x=%d&y=%d", &dado->atual.ponto.coluna, &dado->atual.ponto.linha );
}

void receber_jogadas( char *string, dados *dado ) {
    char *p;
    int i = 0;
    p = strstr( string, "jogadas=" );
    p += strlen( "jogadas=" );

    if( *( p + 1 ) != '\0' ) {
        while( i < MAX &&
               sscanf( p,
                       "%d_%d_%d-",
                       &dado->jogadas[i].jogador,
                       &dado->jogadas[i].ponto.linha,
                       &dado->jogadas[i].ponto.coluna ) ) {
            i++;
            p = strstr( p, "-" ) + 1;
        }
        dado->quantidade_jogadas = i;
    } else {
        dado->jogadas[0].jogador = -1;
    }
}

void print_tabuleiro( int *tabuleiro ) {
    int i, j;

    printf( "<table id=\"tabuleiro\">" );

    for( i = 0; i < LIN; i++ ) {
        printf( "<tr>" );
        for( j = 0; j < COL; j++ ) {
            printf( "<td>%d</td>", tabuleiro[LIN * i + j] );
        }
        printf( "</tr>" );
    }

    printf( "</table>" );
}

void print_jogo( dados *dado, int *tabuleiro ) {
    print_tabuleiro( tabuleiro );
    print_form( dado );
}

void print_form( dados *dado ) {
    int i;

    printf( "<form method=\"POST\" action=\"main\">" );
    printf( "<input type=\"hidden\" name=\"modo\" value=\"10\">" );
    printf( "Coordenada x: <input type=\"number\" name=\"x\"><br>" );
    printf( "Coordenada y: <input type=\"number\" name=\"y\"><br>" );
    printf( "<input type=\"submit\" value=\"Enviar\">" );

    printf( "<input type=\"hidden\" name=\"jogadas\" value=\"" );

    for( i = 0; i < dado->quantidade_jogadas; i++ ) {
        printf( "%d_%d_%d-",
                dado->jogadas[i].jogador,
                dado->jogadas[i].ponto.linha,
                dado->jogadas[i].ponto.coluna );
    }
    printf( "\">" );
}

bool esta_vazio( int *tabuleiro, coordenadas posicao ) {
    return tabuleiro[LIN * posicao.linha + posicao.coluna];
}
