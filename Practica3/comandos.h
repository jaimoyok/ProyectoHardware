#ifndef __COMANDOS__H
#define __COMANDOS__H

#include <stdint.h>

#define DIM_COMANDO 3

enum {
	INICIO_COMANDO = '#',
	FINAL_COMANDO = '!',
};

void buscar_comando(char leido);
#endif