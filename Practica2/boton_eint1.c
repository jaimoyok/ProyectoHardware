#include "boton_eint1.h"
#include "cola.h"
#include "Eventos.h"
#include <LPC210X.H>                            // LPC21XX Peripheral Registers

// variable que se activa al detectar una nueva pulsaci�n
static volatile int eint1_nueva_pulsacion = 0;

void eint1_ISR (void) __irq {
	VICIntEnClr = VICIntEnClr | 0x00008000;	//Deshabilita EINT1
	PINSEL0 		= PINSEL0 & 0xcfffffff;
	cola_guardar_eventos(EV_BOTON,1);	
	EXTINT =  EXTINT | 1;        // clear interrupt flag        
	VICVectAddr = 0;             // Acknowledge Interrupt
	eint1_nueva_pulsacion = 1;
}

void eint1_clear_nueva_pulsacion(void){
	eint1_nueva_pulsacion = 0;
	PINSEL0 = PINSEL0 | 0x20000000;
	VICIntEnable = VICIntEnable | 0x00008000;
};

unsigned int eint1_read_nueva_pulsacion(void){
	return eint1_nueva_pulsacion;
};


void eint1_init (void) {
	eint1_nueva_pulsacion = 0;
	EXTINT =  EXTINT | 1;        // clear interrupt flag

	// configuration of the IRQ slot number 2 of the VIC for EXTINT0
	VICVectAddr3 = (unsigned long)eint1_ISR;          // set interrupt vector in 0
   
	PINSEL0 		= PINSEL0 & 0xcfffffff;	//Se Limpian los bits 29:28
	PINSEL0 		= PINSEL0 | 0x20000000;	// 10 en los bits 29:28 para EINT1

 	// 0x20 bit 5 enables vectored IRQs. 
	// 15 is the number of the interrupt assigned. Number 15 is the EINT1
	VICVectCntl3 = 0x20 | 15;                   
  VICIntEnable = VICIntEnable | 0x00008000; // Enable EXTINT1 Interrupt
}
