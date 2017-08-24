#include "verificacao.h"

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
