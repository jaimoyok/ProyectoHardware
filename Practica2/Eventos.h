#ifndef __EVENTOS__H
#define __EVENTOS__H

#include "cola.h"
#include "stdint.h"
//definicion de eventos
enum{
    EV_TIMER0 = 0,
    EV_TIMER1 = 1,
    EV_BOTON1 = 2,
    EV_BOTON0 = 3,
};

void iniciarOIreversi(void);
void gestionar_eventos(void);

#endif
