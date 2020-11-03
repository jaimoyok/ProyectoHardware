#include "pulsacion.h"
#include <LPC210x.H>                       /* LPC210x definitions */
#include "botones_eint0.h"

static unsigned int estado_pulsacion_0 = NO_PULSADO;


void clear_nueva_pulsacion_0(void){
	eint0_clear_nueva_pulsacion();
}

unsigned int nueva_pulsacion_0(void){
	unsigned int new;
	new = eint0_read_nueva_pulsacion();
	eint0_clear_nueva_pulsacion(); // Las pulsaciones s�lo deben procesarse una vez. Por tanto se pone a 0 despu�s de leerlo
	return new;
}



void actualizar_estado_0(void){
	EXTINT =  EXTINT | 1;        // clear interrupt flag de EINT0     
	if ((EXTINT & 1) == 1){ // si el bot�n est� pulsado, la instrucci�n de arriba no har� nada y EXTINT valdr� 1. Si el bot�n no est� pulsado valdr� 0
		estado_pulsacion_0 = PULSADO;
	}
	else{
		estado_pulsacion_0 = NO_PULSADO;
		// si no est� pulsado se habilitan las interrupciones (antes ya se ha limpiado el de EXTINT)
		VICIntEnable = VICIntEnable | 0x00004000; // Enable EXTINT0 Interrupt (la interrupci�n del bot�n se deshabilita a si misma, al terminar la pulsaci�n hay ue volver a habilitarla)
	}
}

unsigned int leer_estado_0(void){
	actualizar_estado_0();
	return estado_pulsacion_0; 
}