#ifndef __COLA__H
#define __COLA__H
#include "stdint.h"
// tamnyo cola

void cola_iniciar(void);
int tamanyo(void);
uint8_t nuevoEvento(void);
void siguienteEvento(uint32_t *data, uint8_t *evento, uint32_t *time);
uint8_t avanzar(void);
uint8_t cola_guardar_eventos(uint8_t ID_evento, uint32_t auxData);

#endif
