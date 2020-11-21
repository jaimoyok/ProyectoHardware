#include "boton_eint0.h"
#include "Eventos.h"
#include "cola.h"
#include <LPC210X.H>                            // LPC21XX Peripheral Registers

// variable para comprobar que se hacen las interrupciones que deber�an hacerse
// variable que se activa al detectar una nueva pulsaci�n
static volatile int eint0_nueva_pulsacion = 0;

void eint0_ISR (void) __irq {
	VICIntEnClr = VICIntEnClr | 0x00004000;	//Deshabilita EINT0
	PINSEL1 		= PINSEL1 & 0xfffffffC;
	cola_guardar_eventos(EV_BOTON,0);	
	EXTINT =  EXTINT | 1;        // clear interrupt flag        
	VICVectAddr = 0;             // Acknowledge Interrupt
	eint0_nueva_pulsacion = 1;
}

void eint0_clear_nueva_pulsacion(void){
	eint0_nueva_pulsacion = 0;
	PINSEL1 = PINSEL1 | 1;	
	VICIntEnable = VICIntEnable | 0x00004000;
};

unsigned int eint0_read_nueva_pulsacion(void){
	return eint0_nueva_pulsacion;
};

void eint0_init (void) {
	eint0_nueva_pulsacion = 0;
	EXTINT =  EXTINT | 1;        // clear interrupt flag

	// configuration of the IRQ slot number 2 of the VIC for EXTINT0
	VICVectAddr2 = (unsigned long)eint0_ISR;          // set interrupt vector in 0
   
	PINSEL1 		= PINSEL1 & 0xfffffffC;	// Se limpian los bits 1:0
	PINSEL1 		= PINSEL1 | 1;			// 01 los bits 1:0 para EINT0

	// 0x20 bit 5 enables vectored IRQs. 
	// 14 is the number of the interrupt assigned. Number 14 is the EINT0
	VICVectCntl2 = 0x20 | 14;                   
  VICIntEnable = VICIntEnable | 0x00004000; // Enable EXTINT0 Interrupt
}
