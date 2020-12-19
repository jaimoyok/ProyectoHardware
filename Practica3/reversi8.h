#ifndef __REVERSI8__H
#define __REVERSI8__H
#include <stdint.h>
void reversi8_mover_jugador(void);
int reversi8_mover_ia(void);
void reversi8_iniciar(void);
int reversi8_comprobar_movimiento(void);
int reversi8_seleccionar_movimiento(int fila, int columna);
void reversi8_cancelar_movimiento();
#endif
