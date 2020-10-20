#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "timer0.h"
#include "pulsacion.h"

struct time{
    int seconds;
    int miliseconds;
}

void temporizador_iniciar();
void temporizador_alarma(int retardo);
void temporizador_empezar();
struct time temporizador_leer();
struct time temporizador_parar();
void temporizador_interrupción_periodica(int periodo);
void temporizador_rsi();
