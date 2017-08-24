#ifndef VERIFICACAO_H
#define VERIFICACAO_H

#include "datatype.h"
#include <stdbool.h>

bool esta_vazio( const int *tabuleiro, const coordenadas posicao );
bool tabuleiro_cheio( const int *tabuleiro );

int verificar_se_terminou( const int *tabuleiro );
int verificar_linha( const int *tabuleiro );
int verificar_coluna( const int *tabuleiro );
int verificar_diagonal_principal( const int *tabuleiro );
int verificar_diagonal_secundaria( const int *tabuleiro );

#endif
