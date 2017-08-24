#include "prints.h"

void print_resultado( const dados *dado, const int *tabuleiro ) {
    int jogador = verificar_se_terminou( tabuleiro );

    if( jogador == P1 ) {
        printf( "<meta http-equiv=\"refresh\" content=\"0; "
                "url=http://cap.dc.ufscar.br/~740951/jogo/html/vitoria.html\" />" );
    } else if( jogador == P2 ) {
        printf( "<meta http-equiv=\"refresh\" content=\"0; "
                "url=http://cap.dc.ufscar.br/~740951/jogo/html/derrota.html\" />" );
    } else {
        printf( "<meta http-equiv=\"refresh\" content=\"0; "
                "url=http://cap.dc.ufscar.br/~740951/jogo/html/empate.html\" />" );
    }
}

void print_jogo( const dados *dado, const int *tabuleiro ) {
    print_tabuleiro( tabuleiro );
    print_form( dado );
}

void print_tabuleiro( const int *tabuleiro ) {
    unsigned int i, j;

    printf( "<table id=\"tabuleiro\">" );

    for( i = 0; i < LIN; i++ ) {
        printf( "<tr>" );
        for( j = 0; j < COL; j++ ) {
            if( tabuleiro[LIN * i + j] == 0 ) {
                printf( "<td><img onclick=\"setXY(%u,%u);\" "
                        "src=\"http://cap.dc.ufscar.br/~740951/jogo/img/space.png\" "
                        "width=\"150px\" height=\"150px\" style=\"opacity:0.7;\"></td>",
                        j + 1,
                        i + 1 );
            } else if( tabuleiro[LIN * i + j] == P1 ) {
                printf( "<td><img src=\"http://cap.dc.ufscar.br/~740951/jogo/img/X.png\" "
                        "width=\"150px\" height=\"150px\"></td>" );
            } else if( tabuleiro[LIN * i + j] == P2 ) {
                printf( "<td><img src=\"http://cap.dc.ufscar.br/~740951/jogo/img/BOLA.png\" "
                        "width=\"150px\" height=\"150px\"></td>" );
            }
        }
        printf( "</tr>" );
    }

    printf( "</table>" );
}

void print_form( const dados *dado ) {
    int i;

    printf( "<form method=\"POST\" id=\"formulario\" action=\"main\">" );
    printf( "<input type=\"hidden\" id=\"modo\"name=\"modo\" value=\"%u\">", dado->modo_jogo );
    printf( "<input type=\"hidden\" id=\"x\" name=\"x\"><br>" );
    printf( "<input type=\"hidden\" id=\"y\" name=\"y\"><br>" );

    printf( "<input type=\"hidden\" name=\"jogadas\" value=\"" );

    for( i = 0; i < dado->quantidade_jogadas; i++ ) {
        printf( "%u_%u_%u-",
                dado->jogadas[i].jogador,
                dado->jogadas[i].ponto.linha,
                dado->jogadas[i].ponto.coluna );
    }
    printf( "\">" );

    printf( "</form>" );
}
