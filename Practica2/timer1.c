
#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "timer1.h"
#include "pulsacion.h"
#include "cola.h"
#include "Eventos.h"
#include <stdint.h>
// variables para contabilizar el n�mero de interrupciones
static volatile unsigned int timer1_int_count = 0;
static volatile unsigned int timer1_int_count_retardo = 0;
static volatile unsigned int timer1_int_periodo = 0;

unsigned int retardo; //retardo de la alarma
unsigned int periodo; //perido de la alarma ciclica

uint8_t alarmaActivada;
uint8_t periodoActivado;

void timer1_ISR (void) __irq;    // Generate Interrupt 


/* Setup the Timer Counter 1 Interrupt */
void temporizador1_iniciar (void) {
		timer1_int_count = 0;	
	  // configuration of Timer 1
    T1MR0 = 149999;                       // Interrumpe cada 1ms = 3000000-1 counts
    T1MCR = 3;                              // Generates an interrupt and resets the count when the value of MR0 is reached
    T1TCR = 1;                             // Timer1 Enable

    // configuration of the IRQ slot number 1 of the VIC for Timer 1 Interrupt
		VICVectAddr1 = (unsigned long)timer1_ISR;          // set interrupt vector in 1

    // 0x20 bit 5 enables vectored IRQs. 
		// 5 is the number of the interrupt assigned. Number 5 is the Timer 1
		VICVectCntl0 = 0x20 | 5;   
    VICIntEnable = VICIntEnable | 0x00000020; // Enable Timer0 Interrupt                
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

void temporizador_alarma(int _retardo){
  timer1_int_count_retardo = 0;
  retardo=_retardo;
  alarmaActivada = 1;
}



void temporizador_alarma_periodica(int _retardo){
  timer1_int_periodo = 0;
  periodo = _retardo;
  periodoActivado= 1;
}

void temporizador_desactivar_alarma(){
  alarmaActivada=0;
}

/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer1_ISR (void) __irq {
    timer1_int_count++;
    timer1_int_count_retardo++;
    timer1_int_periodo++;
    
    if(periodoActivado){
      if(timer1_int_periodo >= periodo){
        timer1_int_periodo = 0;
        cola_guardar_eventos(EV_TIMER0,0);
      }
    }
    if(alarmaActivada){
      if(timer1_int_count_retardo >= retardo){
        timer1_int_count_retardo = 0;
        alarmaActivada= 0;
        cola_guardar_eventos(EV_TIMER0,1);
      }
    }
    T1IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
}



