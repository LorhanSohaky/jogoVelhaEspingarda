#include "cgi.h"
#include "datatype.h"
#include "prints.h"
#include "verificacao.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

jogada sortear_jogaca_pc( const jogada atual, const int *tabuleiro );

void add_jogadas_ao_tabuleiro( dados *dado, int *tabuleiro );
void add_jogada_as_jogadas( const jogada jogo, dados *dado );

void jogar( dados *dado, int *tabuleiro );
int jogada_usuario( dados *dado, int *tabuleiro );

int main() {
    dados dado;
    char *data;
    int *tabuleiro = calloc( MAX, sizeof( int ) );

    printf( "%s%c%c\n", "Content-Type:text/html;charset=utf-8", 13, 10 );
    printf( "<HEAD><TITLE>Jogo do Oeste</TITLE>" );
    printf( "<META CHARSET=\"UTF-8\"></HEAD>" );
    printf( " <link rel=\"stylesheet\" type=\"text/css\" "
            "href=\"http://cap.dc.ufscar.br/~740951/jogo/css/jogo.css\"> " );
    printf( "<BODY>" );

    data = getenv( "CONTENT_LENGTH" );
    receber_dados( data, &dado );

    jogar( &dado, tabuleiro );

    free( tabuleiro );

    printf( "<script type=\"text/javascript\" "
            "src=\"http://cap.dc.ufscar.br/~740951/jogo/js/script.js\"></script>" );
    printf( "</BODY>" );
    return 0;
}

void jogar( dados *dado, int *tabuleiro ) {
    add_jogadas_ao_tabuleiro( dado, tabuleiro );

    if( dado->modo_jogo == PC ) {
        dado->atual.jogador = P1;

        if( jogada_usuario( dado, tabuleiro ) && !verificar_se_terminou( tabuleiro ) &&
            !tabuleiro_cheio( tabuleiro ) ) {
            jogada pc = sortear_jogaca_pc( dado->atual, tabuleiro );
            add_jogada_as_jogadas( pc, dado ); // Para adicionar a jogada do PC
        }
    } else {
        jogada_usuario( dado, tabuleiro );
    }

    add_jogadas_ao_tabuleiro( dado, tabuleiro );
    if( !tabuleiro_cheio( tabuleiro ) && !verificar_se_terminou( tabuleiro ) ) {
        print_jogo( dado, tabuleiro );
    } else {
        print_resultado( dado, tabuleiro, verificar_se_terminou( tabuleiro ) );
    }
}

int jogada_usuario( dados *dado, int *tabuleiro ) {
    if( dado->atual.ponto.linha < LIN && dado->atual.ponto.coluna < COL &&
        esta_vazio( tabuleiro, dado->atual.ponto ) ) {
        add_jogada_as_jogadas( dado->atual, dado );
        add_jogadas_ao_tabuleiro( dado, tabuleiro );
        return 1;
    }
    return 0;
}

jogada sortear_jogaca_pc( const jogada atual, const int *tabuleiro ) {
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

void add_jogadas_ao_tabuleiro( dados *dado, int *tabuleiro ) {
    int i;

    for( i = 0; i < dado->quantidade_jogadas; i++ ) {
        int linha, coluna;
        linha = dado->jogadas[i].ponto.linha;
        coluna = dado->jogadas[i].ponto.coluna;
        tabuleiro[LIN * linha + coluna] = dado->jogadas[i].jogador;
    }
}

void add_jogada_as_jogadas( const jogada jogo, dados *dado ) {
    dado->jogadas[dado->quantidade_jogadas] = jogo;
    dado->quantidade_jogadas++;
}
