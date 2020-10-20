#ifndef __EVENTOS__H
#define __EVENTOS__H

 #include "cola.h"
 #include "stdint.h"
//definicion de eventos
const char EVENTO1= 1;
const char EVENTO2= 2;
   
static struct Cola_Eventos cola = colaEVacia();

uint8_t cola_guardar_eventos(uint8_t ID_evento, uint32_t auxData);   

#endif 