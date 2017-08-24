#include "cgi.h"
#include "datatype.h"
#include "prints.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool esta_vazio( const int *tabuleiro, const coordenadas posicao );
bool tabuleiro_cheio( const int *tabuleiro );

int verificar_se_terminou( const int *tabuleiro );
int verificar_linha( const int *tabuleiro );
int verificar_coluna( const int *tabuleiro );
int verificar_diagonal_principal( const int *tabuleiro );
int verificar_diagonal_secundaria( const int *tabuleiro );

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

bool esta_vazio( const int *tabuleiro, const coordenadas posicao ) {
    return !tabuleiro[LIN * posicao.linha + posicao.coluna];
}

bool tabuleiro_cheio( const int *tabuleiro ) {
    int i;

    for( i = 0; i < MAX; i++ ) {
        if( tabuleiro[i] == 0 ) {
            return false;
        }
    }

    return true;
}

int verificar_se_terminou( const int *tabuleiro ) {
    int resultado;

    if( ( resultado = verificar_linha( tabuleiro ) ) ) {
        return resultado;
    } else if( ( resultado = verificar_coluna( tabuleiro ) ) ) {
        return resultado;
    } else if( ( resultado = verificar_diagonal_principal( tabuleiro ) ) ) {
        return resultado;
    } else if( ( resultado = verificar_diagonal_secundaria( tabuleiro ) ) ) {
        return resultado;
    }

    return 0;
}

int verificar_linha( const int *tabuleiro ) {
    int i;

    for( i = 0; i < LIN; i++ ) {
        int referencia = tabuleiro[i * LIN + 0 * COL];

        if( referencia ) {
            int j, quantidade = 0;

            for( j = 0; j < COL; j++ ) {
                if( referencia == tabuleiro[i * LIN + j] ) {
                    quantidade++;
                }
            }

            if( quantidade == COL ) {
                return referencia;
            }
        }
    }

    return 0;
}

int verificar_coluna( const int *tabuleiro ) {
    int j;

    for( j = 0; j < COL; j++ ) {
        int referencia = tabuleiro[0 * LIN + j];

        if( referencia ) {
            int i, quantidade = 1;

            for( i = 1; i < LIN; i++ ) {
                if( referencia == tabuleiro[i * LIN + j] ) {
                    quantidade++;
                }
            }

            if( quantidade == LIN ) {
                return referencia;
            }
        }
    }

    return 0;
}

int verificar_diagonal_principal( const int *tabuleiro ) {
    int referencia;

    referencia = tabuleiro[0 * LIN + 0 * COL];

    if( referencia ) {
        int i, soma = 1;

        for( i = 1; i < LIN; i++ ) {
            if( referencia == tabuleiro[i * LIN + i] ) {
                soma++;
            }
        }

        if( soma == LIN ) {
            return referencia;
        }
    }

    return 0;
}

int verificar_diagonal_secundaria( const int *tabuleiro ) {
    int referencia;

    referencia = tabuleiro[0 * LIN + COL - 1];

    if( referencia ) {
        int j, soma = 1;

        for( j = COL - 1; j > 0; j-- ) {
            if( referencia == tabuleiro[( LIN - j ) * LIN + j - 1] ) {
                soma++;
            }
        }

        if( soma == COL ) {
            return referencia;
        }
    }

    return 0;
}
