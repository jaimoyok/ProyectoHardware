#ifndef __REVERSI8__H
#define __REVERSI8__H

#include <stdint.h>

void reversi8_mover_jugador(void);
int reversi8_mover_ia(void);
void reversi8_iniciar(void);
int reversi8_comprobar_movimiento(void);
int reversi8_seleccionar_movimiento(int fila, int columna);
void reversi8_cancelar_movimiento(void);
void mostrarTablero(void);
void mostrarMenu(void);
void reversi8_contar(int *b, int *n);
int reversi8_mov_blancas(void);
int reversi8_mov_negras(void);
#endif

