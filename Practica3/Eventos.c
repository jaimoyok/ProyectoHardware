#include "Eventos.h"
#include "GPIO.h"
#include "Power_management.h"
#include "boton_eint0.h"
#include "boton_eint1.h"
#include "UART0.h"
#include "cola.h"
#include "reversi8.h"
#include "stdint.h"
#include "timer0.h"
#include "timer1.h"
#include "comandos.h"
#include "WT.h"
#include "RTC.h"

enum {
  RETARDO = 250, // periodo de alarma
  PERIODOS = 12  // numero de alarmas que se espera para hacer un movimiento
};
static volatile int usos_timer =
    0;                       // numero de perifericos que necesitan el relog
static volatile int fila;    // fila seleccionada
static volatile int columna; // columna seleccionada
static volatile int ready;
static volatile int cuenta_atras = PERIODOS; // contador confirmacion
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
  // iniciar timers
  temporizador0_iniciar();
  temporizador1_iniciar();
  temporizador0_empezar();
  temporizador1_empezar();
  // iniciar juego
  reversi8_iniciar();
  // iniciar contador para la IA
	uart0_init();
	RTC_init();
	WT_init(50);
}

void aceptar_movimiento() {
  GPIO_escribir(29, 1, 0); // Se limpia la casilla de movimiento incorrecto.
  if (reversi8_comprobar_movimiento()) {
    // si el movimiento es valido se procesa y la ia juega
    reversi8_mover_jugador();
    int aux = clock_gettime(); 
    reversi8_mover_ia();
    tiempoIA = aux - clock_gettime();
  } else {
    // si movimiento no valido se activa bit para indicarlo
    reversi8_cancelar_movimiento();
    GPIO_escribir(29, 1, 1);
  }
}

void controlar_alarmas() {
	static int a =1;
	if(a){
		a = 0;
    temporizador_alarma_periodica(RETARDO);
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
	mostrarMenu();
	print(mostrarTablero());
  while (1) {
    // comprobamos si hay que encender o aparar la alarma??
    controlar_alarmas();
    // esperamos a que exista un evento que procesar
    while (!nuevoEvento()) {
      // si no tenemos que procesar dormimos el procesador
        PM_idle();
    }

    // leemos el siguient evento
    siguienteEvento(&data, &evento, &time);
    switch (evento) {
			case EV_UART0:
				buscar_comando(data);
				break;
			case EV_COMANDO:
				feed_WT();
        switch (data)
        {
        case PASAR:
          if(state == INICIO){
            if (!reversi8_mover_ia()) {
          // Si la IA tambien pasa finaliza la partida.
            state = FIN;
            //tiempoIA = contador(cnt);
             print("****Resultados****"/*mostrarResultado()*/);
            }
            else print(mostrarTablero());
          }
          //TODO:cambiar
          else print("que dise loko");
          break;
        case ACABAR_PARTIDA:
					if(state == ACEPTAR) usos_timer--;
          state = FIN;
          print("Pulse un boton o escriba <#NEW!> para empezar otra partida\n");
          break;
        case NUEVA_PARTIDA:
					if(state == ACEPTAR) usos_timer--;
          state = INICIO;
          reversi8_iniciar();
          print(mostrarTablero());
          break;
        case COMANDO_FALLIDO:
          //TODO:cambiar
          print("Comando no reconocido\n");
          break;
        default:
          if(state== INICIO){
            columna = (data >> 8) & 0xFF;
			      fila = data & 0xFF;
            if(reversi8_seleccionar_movimiento(fila,columna)){
              state = ACEPTAR;
              usos_timer++;
              cuenta_atras = PERIODOS;
							print(mostrarTablero());
							print("Confirme o anule para continuar.\nConfirmacion automatica en 3seg");
            }
            else print("Casilla ocupada\n");
						 break;
          }
          else print("Ya has selecionado movimiento, cancele para introducir otra jugada\n"); 
        }
			
					
				break;
    case EV_BOTON0: {
			feed_WT();
      gestionar_boton0(1);
      switch (state) {
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
        print(mostrarTablero());
        state = INICIO;
        break;
      }

      default:
        break;
      }
      break;
    }
    case EV_BOTON1: {
			feed_WT();
      gestionar_boton1(1);
      switch (state) {
      case INICIO: {
        // El jugador ha pasado de turno.
        if (!reversi8_mover_ia()) {
          // Si la IA tambien pasa finaliza la partida.
          state = FIN;
          //tiempoIA = contador(cnt);
          print("****Resultados****"/*mostrarResultados()*/);
        }
        else print(mostrarTablero());
        break;
      }
      case ACEPTAR: {
        // Se cancela el movimiento realizado.
        usos_timer--;
        reversi8_cancelar_movimiento();
        state = INICIO;
        print(mostrarTablero());
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
        cuenta_atras--;
        if (cuenta_atras == 0) {
          aceptar_movimiento();
					print(mostrarTablero());
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
