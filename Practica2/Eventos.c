 #include "cola.h"
 #include "stdint.h"
 #include "eventos.h"
 
uint8_t cola_guardar_eventos(uint8_t ID_evento, uint32_t auxData){
    uint32_t time = temporizador_leer();
    uint32_t aux = ID_evento << 24 | (auxData & 0x00FFFFFF) ;

    if(insertarEvento(&cola, aux,time))return 1;
    return 0;
}   
