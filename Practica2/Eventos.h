#ifndef __EVENTOS__H
#define __EVENTOS__H

#include "cola.h"
#include "stdint.h"
//definicion de eventos
enum{
    EV_TIMER0 = 0,
    EV_TIMER1 = 1,
    EV_BOTON = 2,
};

int esperar_movimiento(void);
int leer_pulsaciones(void);
int leer_move(void);
int leer_fila(void);
int leer_columna(void);
void iniciarOIreversi(void);


#endif
