
#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "timer0.h"
#include "pulsacion.h"
#include "cola.h"
#include "Eventos.h"
#include <stdint.h>
// variables para contabilizar el n�mero de interrupciones
static volatile unsigned int timer0_int_count = 0;
static volatile unsigned int timer0_int_count_retardo = 0;
static volatile unsigned int timer0_int_periodo = 0;

unsigned int retardo; //retardo de la alarma
unsigned int periodo; //perido de la alarma ciclica

uint8_t alarmaActivada;
uint8_t periodoActivado;

void timer0_ISR (void) __irq;    // Generate Interrupt 


/* Setup the Timer Counter 0 Interrupt */
void temporizador0_iniciar (void) {
		timer0_int_count = 0;	
	// configuration of Timer 0
		T0MR0 = 149999; 
    //T0MR0 = 2999990;                       // Interrumpe cada 1ms = 3000000-1 counts
    T0MCR = 3;                              // Generates an interrupt and resets the count when the value of MR0 is reached
    T0TCR = 1;                             // Timer0 Enable
    // configuration of the IRQ slot number 0 of the VIC for Timer 0 Interrupt
		VICVectAddr0 = (unsigned long)timer0_ISR;          // set interrupt vector in 0
    // 0x20 bit 5 enables vectored IRQs. 
		// 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
		VICVectCntl0 = 0x20 | 4;   
    VICIntEnable = VICIntEnable | 0x00000010;                  // Enable Timer0 Interrupt                
}

void temporizador0_empezar(void) {
  timer0_int_count = 0;
  
}

unsigned int temporizador0_leer(void){
	return timer0_int_count;
};

void temporizador0_parar(void) {
  VICIntEnClr = VICIntEnClr | 0x00000010;
}

void temporizador_alarma(int _retardo){
  timer0_int_count_retardo = 0;
  retardo=_retardo;
  periodoActivado = 1;
}



void temporizador_alarma_periodica(int _retardo){
  timer0_int_periodo = 0;
  periodo = _retardo;
  alarmaActivada = 1;
}

void temporizador_desactivar_alarma(){
  alarmaActivada=0;
}

/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer0_ISR (void) __irq {
    timer0_int_count++;
    timer0_int_count_retardo++;
    timer0_int_periodo++;
    
    if(alarmaActivada){
      if(timer0_int_count_retardo >= retardo){
        timer0_int_count_retardo = 0;
        cola_guardar_eventos(EV_TIMER0,0);
      }
    }
    if(periodoActivado){
      if(timer0_int_count_retardo >= retardo){
        timer0_int_count_retardo = 0;
        periodoActivado = 0;
        cola_guardar_eventos(EV_TIMER0,1);
      }
    }
    T0IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
}



