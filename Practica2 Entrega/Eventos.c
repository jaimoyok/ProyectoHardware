#include "Eventos.h"
#include "GPIO.h"
#include "Power_management.h"
#include "boton_eint0.h"
#include "boton_eint1.h"
#include "cola.h"
#include "reversi8.h"
#include "stdint.h"
#include "timer0.h"
#include "timer1.h"

enum {
  RETARDO = 250, // periodo de alarma
  PERIODOS = 12  // numero de alarmas que se espera para hacer un movimiento
};
static volatile int usos_timer =
    0;                       // numero de perifericos que necesitan el relog
static volatile int fila;    // fila seleccionada
static volatile int columna; // columna seleccionada
static volatile int cuenta_atras = PERIODOS; // contador confirmacion
int cnt;      // identificador del contador preciso
int tiempoIA; // guarda los milisegundos que tarda la IA en hacer un movimiento

typedef enum { no_pulsado = 0, pulsado = 1 } estado_boton_t;

typedef enum { INICIO, ACEPTAR, MOVER, FIN } Estado;

void iniciarOIreversi() {
  // activar perifericos
  eint0_init();
  eint1_init();
  GPIO_iniciar();
  GPIO_marcar_salida(0, 32);
  GPIO_escribir(24, 8, 0);
  GPIO_escribir(0, 3, 0);
  GPIO_escribir(8, 3, 0);
  // Fila y columna
  GPIO_marcar_entrada(0, 3);
  GPIO_marcar_entrada(8, 3);
  // iniciar timers
  temporizador0_iniciar();
  temporizador1_iniciar();
  temporizador0_empezar();
  temporizador1_empezar();
  // iniciar juego
  reversi8_iniciar();
  // iniciar contador para la IA
  cnt = iniciar_contador();
}

void leer_movimiento() {
  fila = GPIO_leer(0, 3);
  columna = GPIO_leer(8, 3);
}

void aceptar_movimiento() {
  GPIO_escribir(29, 1, 0); // Se limpia la casilla de movimiento incorrecto.
  if (reversi8_comprobar_movimiento(fila, columna)) {
    // si el movimiento es valido se procesa y la ia juega
    reversi8_mover_jugador(fila, columna);
    resetear_contador(cnt);
    reversi8_mover_ia();
    tiempoIA = contador(cnt);
  } else {
    // si movimiento no valido se activa bit para indicarlo
    GPIO_escribir(29, 1, 1);
    limpiar_casilla(fila, columna); // Se asegura que quede a 0
  }
}

void controlar_alarmas() {
  static int alarma_activada = 0;
  if (usos_timer > 0 &&
      alarma_activada == 0) { // Numero pulsados > 0 (algo requiere alarma)
    alarma_activada = 1;
    temporizador_alarma_periodica(RETARDO);
  } else if (usos_timer == 0 &&
             alarma_activada ==
                 1) { // Numero pulsaciones = 0 (no se requiere alarma)
    alarma_activada = 0;
    temporizador_desactivar_alarma();
  }
}

// gestiona la pulsacion del boton 0(pin 16)
void gestionar_boton0(uint8_t interrupcion_boton) {

  static estado_boton_t estado = no_pulsado;
  // si no esta pulsado ni ha llegado interrupcion no se hace nada
  if (interrupcion_boton == 0 && estado == no_pulsado) {
    return;
  }

  // ha habido interrupcion se pasa al estado pulsado
  if (estado == no_pulsado) {
    estado = pulsado;
    usos_timer++;
  } else {
    // Si boton se ha dejado de pulsar
    if (!eint0_esta_pulsado()) {
      // activamos las interrupciones otra vez
      eint0_clear_nueva_pulsacion();
      estado = no_pulsado;
      usos_timer--;
    }
  }
}

// gestiona la pulsacion del boton 1(pin 14)
void gestionar_boton1(uint8_t interrupcion_boton) {

  static estado_boton_t estado = no_pulsado;
  // si no esta pulsado ni ha llegado interrupcion no se hace nada
  if (interrupcion_boton == 0 && estado == no_pulsado) {
    return;
  }

  // ha habido interrupcion se pasa al estado pulsado
  if (estado == no_pulsado) {
    estado = pulsado;
    usos_timer++;
  } else {
    // Si boton se ha dejado de pulsar
    if (!eint1_esta_pulsado()) {
      // activamos las interrupciones otra vez
      eint1_clear_nueva_pulsacion();
      estado = no_pulsado;
      usos_timer--;
    }
  }
}

// hace parpadear el pin 31
void gestionar_led() {
  static int estado = 0;
  GPIO_escribir(31, 1, estado);
  estado = !estado;
}

// gestiona los perifericos y el juego reversi
void gestionar_eventos() {
  Estado state = INICIO;
  uint8_t evento = 0;
  uint32_t data = 0;
  uint32_t time = 0;
  while (1) {
    // comprobamos si hay que encender o aparar la alarma??
    controlar_alarmas();
    // esperamos a que exista un evento que procesar
    while (!nuevoEvento()) {
      // si estamos en el estado inicio dormimos el procesador
      if (state == INICIO) {
        //PM_power_down(); //TODO:cambiar a power_down
				PM_idle();
      }
      // si no necesitamos recibir los eventos del timer
      else {
        PM_idle();
      }
    }
    // leemos el siguient evento
    siguienteEvento(&data, &evento, &time);
    switch (evento) {
    case EV_BOTON0: {
      gestionar_boton0(1);
      switch (state) {
      case INICIO: {
        // Se ha realizado un movimiento.
        // A espera de confirmación temporal (3s) o por botón.
        state = ACEPTAR;
        usos_timer++;
        leer_movimiento();
        cuenta_atras = PERIODOS;
        break;
      }
      case ACEPTAR: {
        // El usuario ha confirmado su movimiento.
        // Se coloca la ficha y turno de la IA.
        aceptar_movimiento();
        state = INICIO;
        usos_timer--;
        break;
      }
      case FIN: {
        // Se ha terminado la partida y comienza una nueva.
        reversi8_iniciar();
        state = INICIO;
        break;
      }

      default:
        break;
      }
      break;
    }
    case EV_BOTON1: {
      gestionar_boton1(1);
      switch (state) {
      case INICIO: {
        // El jugador ha pasado de turno.
        resetear_contador(cnt);
        if (!reversi8_mover_ia()) {
          // Si la IA tambien pasa finaliza la partida.
          state = FIN;
          //tiempoIA = contador(cnt);
        }
        break;
      }
      case ACEPTAR: {
        // Se cancela el movimiento realizado.
        usos_timer--;
        limpiar_casilla(
            fila, columna); // Se asegura de que se no se modifica la casilla
        state = INICIO;
        break;
      }
      case FIN: {
        // Se ha terminado la partida y comienza una nueva.
        reversi8_iniciar();
        state = INICIO;
        break;
      }
      default:
        break;
      }
      break;
    }

    case EV_TIMER0: {
      gestionar_boton1(0);
      gestionar_boton0(0);
      gestionar_led();
      switch (state) {
      case ACEPTAR: {
        parpadea(fila, columna);
        cuenta_atras--;
        if (cuenta_atras == 0) {
          aceptar_movimiento();
          state = INICIO;
          usos_timer--;
        }
        break;
      }
      default:
        break;
      }
      break;
    }
    }
    avanzar();
  }
}
