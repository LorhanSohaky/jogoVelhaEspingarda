#ifndef PRINTS_H
#define PRINTS_H

#include "datatype.h"

void print_tabuleiro( const int *tabuleiro );
void print_jogo( const dados *dado, const int *tabuleiro );
void print_form( const dados *dado );
void print_resultado( const dados *dado, const int *tabuleiro, int jogador );

#endif
