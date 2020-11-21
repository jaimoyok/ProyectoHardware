#ifndef __COLA__H
#define __COLA__H
#include "stdint.h"
//tamnyo cola

void cola_iniciar();
int tamanyo();
uint8_t nuevoEvento();
void siguienteEvento(uint32_t *data, uint8_t *evento, uint32_t *time);
uint8_t avanzar();
uint8_t cola_guardar_eventos(uint8_t ID_evento, uint32_t auxData);

#endif
