#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
jogada sortear_jogaca_pc( jogada atual, int *tabuleiro );

void print_tabuleiro( int *tabuleiro );
void print_jogo( dados *dado, int *tabuleiro );
void print_form( dados *dado );

void receber_dados( char *data, dados *dado );
void receber_modo_jogo( char *string, dados *dado );
void receber_jogada_atual( char *string, dados *dado );
void receber_jogadas( char *string, dados *dado );

void add_jogadas_ao_tabuleiro( dados *dado, int *tabuleiro );
void add_jogada_as_jogadas( jogada jogo, dados *dado );

int main() {
    dados dado;
    char *data;
    int *tabuleiro = calloc( MAX, sizeof( int ) );

    printf( "%s%c%c\n", "Content-Type:text/html;charset=utf-8", 13, 10 );
    printf( "<TITLE>Jogo da velha</TITLE>\n" );

    data = getenv( "CONTENT_LENGTH" );
    receber_dados( data, &dado );

    add_jogadas_ao_tabuleiro( &dado, tabuleiro );

    if( dado.modo_jogo == PC ) {
        jogada pc = sortear_jogaca_pc( dado.atual, tabuleiro );

        dado.atual.jogador = P1;

        add_jogada_as_jogadas( pc, &dado );
    }

    if( dado.atual.ponto.linha != -1 && dado.atual.ponto.coluna != -1 ) {
        if( esta_vazio( tabuleiro, dado.atual.ponto ) ) {
            add_jogada_as_jogadas( dado.atual, &dado );
            add_jogadas_ao_tabuleiro( &dado, tabuleiro );
        }
    }

    print_jogo( &dado, tabuleiro );

    free( tabuleiro );

    return 0;
}

jogada sortear_jogaca_pc( jogada atual, int *tabuleiro ) {
    jogada pc;

    pc.jogador = P2;

    srand( time( NULL ) );

    do {
        pc.ponto.linha = rand() % LIN;
        pc.ponto.coluna = rand() % COL;
    } while( ( pc.ponto.linha == atual.ponto.linha && pc.ponto.coluna == atual.ponto.coluna ) ||
             !esta_vazio( tabuleiro, pc.ponto ) );

    return pc;
}

void receber_dados( char *data, dados *dado ) {
    unsigned int tamanho;
    char *string;

    dado->atual.ponto.coluna = -1;
    dado->atual.ponto.linha = -1;

    if( data != NULL && sscanf( data, "%d", &tamanho ) == 1 ) {
        string = calloc( tamanho + 1, sizeof( char ) );
        if( !string ) {
            return;
        }

        fgets( string, tamanho + 1, stdin );

        receber_modo_jogo( string, dado );
        receber_jogada_atual( string, dado );
        receber_jogadas( string, dado );

        dado->atual.jogador = determinar_jogador( dado );

        free( string );
    }
}

unsigned int determinar_jogador( dados *dado ) {
    if( dado->quantidade_jogadas == 0 ||
        dado->jogadas[dado->quantidade_jogadas - 1].jogador % 2 == 0 ) {
        return P1;
    } else {
        return P2;
    }
}

void add_jogadas_ao_tabuleiro( dados *dado, int *tabuleiro ) {
    int i;
    int linha, coluna;
    for( i = 0; i < dado->quantidade_jogadas; i++ ) {
        linha = dado->jogadas[i].ponto.linha;
        coluna = dado->jogadas[i].ponto.coluna;
        tabuleiro[LIN * linha + coluna] = dado->jogadas[i].jogador;
    }
}

void add_jogada_as_jogadas( jogada jogo, dados *dado ) {
    dado->jogadas[dado->quantidade_jogadas] = jogo;
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
    if( p ) {
        if( !sscanf( p, "x=%d&y=%d", &dado->atual.ponto.coluna, &dado->atual.ponto.linha ) ) {
            dado->atual.ponto.coluna = -1;
            dado->atual.ponto.linha = -1;
        }
    }
}

void receber_jogadas( char *string, dados *dado ) {
    char *p;
    int i = 0;
    p = strstr( string, "jogadas=" );
    p += strlen( "jogadas=" );
    if( *( p + 1 ) != '\0' ) {
        while( sscanf( p,
                       "%d_%d_%d-",
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
    printf( "<input type=\"hidden\" name=\"modo\" value=\"2\">" );
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
    return !tabuleiro[LIN * posicao.linha + posicao.coluna];
}
