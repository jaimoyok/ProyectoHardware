#include "cola.h"
#include "GPIO.h"
#include "stddef.h "
#include "stdint.h"
#include "timer0.h"
#include "timer1.h"

enum { DIM = 512 };

typedef struct {
  uint32_t id_aux;
  uint32_t timestamp;
} elem_cola;

typedef struct {
  elem_cola data[DIM];
  size_t head, tail, n;
} cola_t;

volatile cola_t cola;

// inicia cola
void cola_iniciar() {
  cola.head = 0;
  cola.tail = 0;
  cola.n = 0;
  cola_guardar_eventos(0, 0);
}

// devuelve el tamnyo de la cola
size_t cola_tamanyo() { return cola.n; }

// inserta un envento en la cola si es posible y devuelve 1
// si no es posible se queda en un bucle infinito
uint8_t insertarEvento(elem_cola e) {
  // GPIO
  if (cola.n == DIM) {
    GPIO_marcar_salida(30, 1);
    GPIO_escribir(30, 1, 1);
    while (1)
      ;
  }
  cola.n++;
  cola.data[cola.tail] = e;
  cola.tail++;
  return 1;
}

// devuelve 1 si existe un evento que procesar
// si no devuelve un 0
uint8_t nuevoEvento() { return cola.n != 0; }

// rellena data, evento y time con los datos del primer evento de la lista
void siguienteEvento(uint32_t *data, uint8_t *evento, uint32_t *time) {
  if (0 == cola.n) {
    while (1)
      ;
  }
  *evento = cola.data[cola.head].id_aux >> 24;
  *time = cola.data[cola.head].timestamp;
  *data = cola.data[cola.head].id_aux & 0x00FFFFFF;
}

// descarta el primer evento de la cola
uint8_t avanzar() {
  if (cola.n == 0)
    return 0;
  cola.head++;
  cola.n--;
  return 1;
}

uint8_t cola_guardar_eventos(uint8_t ID_evento, uint32_t auxData) {
  elem_cola e;
  e.timestamp = clock_gettime();
  // e.timestamp = 0;
  e.id_aux = ID_evento << 24 | (auxData & 0x00FFFFFF);
  if (insertarEvento(e))
    return 1;
  return 0;
}
