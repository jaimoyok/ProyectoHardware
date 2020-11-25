#ifndef __REVERSI8__H
#define __REVERSI8__H
#include <stdint.h>
void reversi8_mover_jugador(int fila, int columna);
int reversi8_mover_ia(void);
void reversi8_iniciar(void);
void parpadea(int fila,int  columna);
int reversi8_comprobar_movimiento(int8_t fila, int8_t columna);
void limpiar_casilla(int fila, int columna);
#endif
