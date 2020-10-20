#ifndef __COLA__H
#define __COLA__H
#include "stdint.h"
//tamnyo cola
enum{ DIM = 32;}


struct Cola_Eventos{
        uint8_t eventos[DIM];
        uint32_t time[DIM];
        int inicio,final,ultimoMirado;

    
};

static struct Cola_Eventos cola = colaEVacia();
struct Cola_Eventos colaEVacia();
int tamanyo(struct Cola_Eventos *c);
uint8_t nuevoEvento(struct Cola_Eventos *c);
uint8_t insertarEvento(struct Cola_Eventos *c, uint32_t evento, uint32_t time);
void siguienteEvento(struct Cola_Eventos *c, uint8_t *evento, uint32_t *time);
uint8_t avanzar(struct Cola_Eventos *c);



#endif 