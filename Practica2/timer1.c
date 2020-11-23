
#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "timer1.h"
#include "pulsacion.h"
#include "cola.h"
#include "Eventos.h"
#include <stdint.h>
// variables para contabilizar el n�mero de interrupciones
const int MAX_CONT
static volatile unsigned long timer1_int_count = 0;
static volatile int contadores = 0;
static volatile long inicio_contadores[MAX_CONT];
void timer1_ISR (void) __irq;    // Generate Interrupt


/* Setup the Timer Counter 1 Interrupt */
void temporizador1_iniciar (void) {
		APBDIV |= 1;
		timer1_int_count = 0;

	  // configuration of Timer 1
    T1MR0 = 599999;                      // Interrumpe cada 0,1ms
    T1MCR = 3;                              // Generates an interrupt and resets the count when the value of MR0 is reached
    T1TCR = 1;                             // Timer1 Enable

    // configuration of the IRQ slot number 1 of the VIC for Timer 1 Interrupt
		VICVectAddr1 = (unsigned long)timer1_ISR;          // set interrupt vector in 1

    // 0x20 bit 5 enables vectored IRQs.
		// 5 is the number of the interrupt assigned. Number 5 is the Timer 1
		VICVectCntl1 = 0x20 | 5;
    VICIntEnable = VICIntEnable | 0x00000020; // Enable Timer1 Interrupt
}

void temporizador1_empezar(void) {
  timer1_int_count = 0;
}

unsigned int temporizador1_leer(void){
	return timer1_int_count;
};

void temporizador1_parar(void) {
  VICIntEnClr = VICIntEnClr | 0x00000020;
}

//inicia un contador y devuelve su identificador
//devuelve menos uno si no se puede iniciar
int iniciar_contador(){
	if (contadores == MAX_CONT)return -1;
	inicio_contadores[contadores] = timer1_int_count;
	contadores ++;
	return contadores-1;
}

//resetea el contador con identificador numContador
//si este no se ha inicado no hace nada
void resetear_contador(int numContador){
	if (numContador > contadores)return;
	inicio_contadores[numContador] = timer1_int_count;
}

//devuelve el valor actual del contador con identificador numContador
//devuelve -1 si este no se ha iniciado
int contador(int numContador){
	if (numContador > contadores)return -1;
	return inicio_contadores[numContador] - timer1_int_count;
}

/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer1_ISR (void) __irq {
    timer1_int_count++;
    T1IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
}
