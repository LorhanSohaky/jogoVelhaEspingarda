#include "cgi.h"
#include "jogo.h"
#include <stdio.h>
#include <stdlib.h>

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
