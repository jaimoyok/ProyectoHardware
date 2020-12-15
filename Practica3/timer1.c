#include "timer1.h"
#include "Eventos.h"
#include "cola.h"
#include <LPC210X.H> // LPC21XX Peripheral Registers
#include <stdint.h>

// variables para contabilizar el n�mero de interrupciones
static volatile unsigned long timer1_int_count = 0;
const unsigned int MAX_TICS= 0xFFFFFFFF;
void timer1_ISR(void) __irq; // Generate Interrupt

/* Setup the Timer Counter 1 Interrupt */
void temporizador1_iniciar(void) {
  VPBDIV |= 1;
  timer1_int_count = 0;

  // configuration of Timer 1
	T1PR= MAX_TICS -1;
  T1MCR = 3; // Generates an interrupt and resets the count when the value of
             // MR0 is reached
  T1TCR = 1; // Timer1 Enable

  // configuration of the IRQ slot number 1 of the VIC for Timer 1 Interrupt // set interrupt vector in 1

  // 0x20 bit 5 enables vectored IRQs.
  // 5 is the number of the interrupt assigned. Number 5 is the Timer 1
}

// inicia el contador principal
void temporizador1_empezar(void) { 
	timer1_int_count = 0;
	T1MCR = 3; // Generates an interrupt and resets the count when the value of
             // MR0 is reached
  T1TCR = 1; // Timer1 Enable
}

// devuelve el valor actual del contador principal
unsigned int temporizador1_leer(void) { return T1TC * MAX_TICS + T1PC; };

// para el temporizador y devuelve el valor que toma el contador principañ
unsigned int temporizador1_parar(void) {
  VICIntEnClr = VICIntEnClr | 0x00000020;
  return temporizador1_leer();
}

/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer1_ISR(void) __irq {
  timer1_int_count++;
  T1IR = 1;        // Clear interrupt flag
  VICVectAddr = 0; // Acknowledge Interrupt
}

uint32_t __SWI_0 (void) {
	return temporizador1_leer();
}
