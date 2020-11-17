#include "boton_eint1.h"
#include "cola.h"
#include "Eventos.h"
#include <LPC210X.H>                            // LPC21XX Peripheral Registers

// variable que se activa al detectar una nueva pulsaci�n
static volatile int eint1_nueva_pulsacion = 0;
static volatile int eint1_count = 0;

void eint1_ISR (void) __irq {
	VICIntEnable = VICIntEnClr | 0x00010000;
	cola_guardar_eventos(EV_BOTON,1);
	EXTINT =  EXTINT | 1;        // clear interrupt flag        
	VICVectAddr = 0;             // Acknowledge Interrupt
	eint1_nueva_pulsacion = 1;
}

void eint1_clear_nueva_pulsacion(void){
	eint1_nueva_pulsacion = 0;
	VICIntEnable = VICIntEnable | 0x00010000;
};

unsigned int eint1_read_nueva_pulsacion(void){
	return eint1_nueva_pulsacion;
};

unsigned int eint1_read_count(void){
	return eint1_count;
};

void eint1_init (void) {
// NOTA: seg�n el manual se puede configurar c�mo se activan las interrupciones: por flanco o nivel, alta o baja. 
// Se usar�an los registros EXTMODE y EXTPOLAR. 
// Sin embargo parece que el simulador los ignora y no aparecen en la ventana de ocnfiguraci�n de EXT Interrupts
// configure EXTINT0 active if a rising-edge is detected
//	EXTMODE 	=	1; //1 edge, 0 level
//	EXTPOLAR	=	1; // 1 high, rising-edge; 0 low, falling-edge
//  prueba = EXTMODE;
	eint1_nueva_pulsacion = 0;
	EXTINT =  EXTINT | 1;        // clear interrupt flag     	
	// configuration of the IRQ slot number 2 of the VIC for EXTINT0
	VICVectAddr2 = (unsigned long)eint1_ISR;          // set interrupt vector in 0
    // 0x20 bit 5 enables vectored IRQs. 
		// 14 is the number of the interrupt assigned. Number 14 is the EINT0 (see table 40 of the LPC2105 user manual  
	PINSEL0 		= PINSEL0 & 0xcfffffff;	//Sets bits 0 and 1 to 0
	PINSEL0 		= PINSEL0 | 0x20000000;

	VICVectCntl2 = 0x20 | 14;                   
  	VICIntEnable = VICIntEnable | 0x00010000;                  // Enable EXTINT1 Interrupt
}
