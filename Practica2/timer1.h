#ifndef __TIMER1__H
#define __TIMER1__H

void temporizador1_iniciar (void);
void temporizador1_empezar(void);
unsigned int temporizador1_leer(void);
void temporizador1_parar(void);
void temporizador_alarma(int retardo);
void temporizador_alarma_periodica(int _retardo);
void temporizador_desactivar_alarma();

#endif