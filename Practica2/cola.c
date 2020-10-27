 #include "cola.h"
 #include "stdint.h"

 enum{ DIM = 32};


typedef struct {
uint32_t id_aux;
uint32_t timestamp;
} elem_cola;

typedef struct {
elem_cola data[DIM];
size_t head, tail, n;
} cola_t;

cola_t cola;

//inicia cola
 void cola_iniciar(){
     cola.head = 0;
     cola.tail = 0;
     cola.n = 0
    }


size_t cola_tamanyo(){
    return cola.n;
}


uint8_t insertarEvento(elem_cola e){
    //GPIO
    if (cola.n == DIM) return 0;
    cola.n++;
    cola.data[cola.tail]=e;
    cola.tail++;
    return 1;
}
uint8_t nuevoEvento(){
    return cola.n == DIM;
}
void siguienteEvento(uint32_t *data, uint8_t *evento, uint32_t *time){
    *evento =  cola.data[cola.head].id_aux >> 24;
    *time = cola.data[cola.head].timestamp;
    *data = cola.data[cola.head].id_aux & 0x00FFFFFF;
}
uint8_t avanzar(){
    if ( cola.n == 0)return 0;
    cola.head++;
    cola.n--;
    return 1;
 }

  
uint8_t cola_guardar_eventos(uint8_t ID_evento, uint32_t auxData){
    elem_cola e;
    e.timestamp = temporizador_leer();
    e.id_aux = ID_evento << 24 | (auxData & 0x00FFFFFF) ;
    if(insertarEvento(e))return 1;
    return 0;
}   
