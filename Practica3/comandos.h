#ifndef __COMANDOS__H
#define __COMANDOS__H

#include <stdint.h>

#define DIM_COMANDO 10
//valores mayores a 2^16 para no solapar las filas y columnas
enum{
	PASAR = 65537,
	ACABAR_PARTIDA = 65538,
	NUEVA_PARTIDA = 65539,
	COMANDO_FALLIDO = 65540,
};
enum {
	INICIO_COMANDO = '#',
	FINAL_COMANDO = '!',
};

void buscar_comando(char leido);
#endif
