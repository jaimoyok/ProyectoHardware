#ifndef __COMANDOS__H
#define __COMANDOS__H

#define DIM_COMANDO 3
enum {
	INICIO_COMANDO = '#',
	FINAL_COMANDO = '!',
};

#include <stdint.h>

void gestionar_lectura (char leido);
#endif