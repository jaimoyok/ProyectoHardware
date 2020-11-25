#ifndef __TIMER1__H
#define __TIMER1__H

void temporizador1_iniciar (void);
void temporizador1_empezar(void);
unsigned int temporizador1_leer(void);
unsigned int temporizador1_parar(void);
int iniciar_contador(void);
void resetear_contador(int numContador);
int contador(int numContador);

#endif
