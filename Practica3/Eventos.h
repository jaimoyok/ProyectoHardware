#ifndef __EVENTOS__H
#define __EVENTOS__H

#include "cola.h"

// definicion de eventos
enum {
  EV_TIMER0 = 0,
  EV_TIMER1 = 1,
  EV_BOTON1 = 2,
  EV_BOTON0 = 3,
	EV_UART0 = 4,
	EV_COMANDO = 5,
};

void iniciarOIreversi(void);
void gestionar_eventos(void);

#endif
