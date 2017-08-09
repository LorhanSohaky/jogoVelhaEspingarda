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

bool esta_vazio( const int *tabuleiro, const coordenadas posicao );
bool tabuleiro_cheio( const int *tabuleiro );
unsigned int determinar_jogador( const dados *dado );

int verificar_se_terminou( const int *tabuleiro );
int verificar_linha( const int *tabuleiro );
int verificar_coluna( const int *tabuleiro );
int verificar_diagonal_principal( const int *tabuleiro );
int verificar_diagonal_secundaria( const int *tabuleiro );

jogada sortear_jogaca_pc( const jogada atual, const int *tabuleiro );

void print_tabuleiro( const int *tabuleiro );
void print_jogo( const dados *dado, const int *tabuleiro );
void print_form( const dados *dado );
void print_resultado( const dados *dado, const int *tabuleiro );

void receber_dados( const char *data, dados *dado );
void receber_modo_jogo( const char *string, dados *dado );
void receber_jogada_atual( const char *string, dados *dado );
void receber_jogadas( const char *string, dados *dado );

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
        print_resultado( dado, tabuleiro );
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

unsigned int determinar_jogador( const dados *dado ) {
    if( dado->quantidade_jogadas == 0 ||
        dado->jogadas[dado->quantidade_jogadas - 1].jogador % 2 == 0 ) {
        return P1;
    } else {
        return P2;
    }
}
