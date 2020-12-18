#include "comandos.h"
#include "Eventos.h"

void buscar_comando (char leido) {
	static int i = 0;
	static char comando[DIM_COMANDO];
	uint32_t dato;
	
	switch(leido) {
		case INICIO_COMANDO:	// Comienzo de un comando '#'
			i = 0;
			break;
		case FINAL_COMANDO:	// Final de un comando '!'
				dato = comando[2] | (comando[1] << 8)| (comando[0] << 16); //Conversion a formato de evento
				cola_guardar_eventos(EV_COMANDO, dato);
			break;
		default: //Letra del comando
			if (i < DIM_COMANDO) {
				comando[i] = leido;	
				i++;
			}
	}

}
