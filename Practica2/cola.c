 #include "cola.h"
 #include "stdint.h"
 
 
 struct Cola_Eventos colaEVacia(){
     struct Cola_Eventos c;
     c.inicio=0;
     c.final=0;
     c.ultimoMirado=0;
     return c;
 } 


int tamanyo(struct Cola_Eventos *c){
    return (c->final - c->inicio)%DIM;
}

uint8_t insertarEvento(struct Cola_Eventos *c, uint32_t evento, uint32_t time){
    if ((c->final+1)%32 == c->inicio) return 0;
    c->final++;
    c->eventos [c->final] = evento;
    c->time[c->final] = time;
    return 1;
}
uint8_t nuevoEvento(struct Cola_Eventos *c){
    if (c->final == c->ultimoMirado) return 0;
    c->ultimoMirado = c->final;
    return 1;
}
void siguienteEvento(struct Cola_Eventos *c, uint8_t *evento, uint32_t *time){
    *evento =  c->eventos[c->inicio];
    *time = c->time[c->inicio];
}
uint8_t avanzar(struct Cola_Eventos *c){
    if ((c->inicio+1)%32 == c->final) return 0;
    c->inicio++;
    return 1;
 }

  
uint8_t cola_guardar_eventos(uint8_t ID_evento, uint32_t auxData){
    uint32_t time = temporizador_leer();
    uint32_t aux = ID_evento << 24 | (auxData & 0x00FFFFFF) ;

    if(insertarEvento(&cola, aux,time))return 1;
    return 0;
}   
