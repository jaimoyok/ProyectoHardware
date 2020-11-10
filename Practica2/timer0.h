#ifndef __TIMER0__H
#define __TIMER0__H

void temporizador0_iniciar (void);
void temporizador0_empezar(void);
unsigned int temporizador0_leer(void);
void temporizador0_parar(void);
void temporizador_alarma(int retardo);
void temporizador_alarma_periodica(int _retardo);
void temporizador_desactivar_alarma();

#endif