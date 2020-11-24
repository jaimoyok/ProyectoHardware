#include "cola.h"
#include "stdint.h"
#include "Eventos.h"
#include "boton_eint0.h"
#include "boton_eint1.h"
#include "timer0.h"
#include "timer1.h"
#include "GPIO.h"
#include "Power_management.h"
#include "reversi8.h"

const int RETARDO = 250; //periodo de comprobacion
const int PERIODOS = 12;  //numero de alarmas que se espera para hacer un movimiento
static volatile int numero_pulsados = 0;
static volatile int pulsacion = 0;
static volatile int mover = 0;
static volatile int fila;
static volatile int columna;
static volatile int cuenta_atras = PERIODOS;
static volatile int aceptando = 0;

typedef enum {
    no_pulsado = 0,
    pulsado = 1
} estado_boton_t;

typedef enum {
    INICIO,
    ACEPTAR,
    MOVER,
    FIN
} Estado;

void iniciarOIreversi() {
    //activar perifericos
    eint0_init();
    eint1_init();
    GPIO_iniciar();
    GPIO_marcar_salida(0, 32);
    GPIO_escribir(24, 8, 0);
    GPIO_escribir(0, 3, 0);
    GPIO_escribir(8, 3, 0);
    //Botones
    //GPIO_marcar_entrada(16, 1);
    //GPIO_marcar_entrada(14, 1);
    //Fila y columna
    GPIO_marcar_entrada(0, 3);
    GPIO_marcar_entrada(8, 3);
    temporizador0_iniciar();
    temporizador1_iniciar();
	  temporizador0_empezar();
    temporizador1_empezar();
    reversi8_iniciar();
}

void leer_movimiento() {
    fila = GPIO_leer(0, 3);
    columna = GPIO_leer(8, 3);
}

void aceptar_movimiento() {
    GPIO_escribir(29,1,0); //Se limpia la casilla de movimiento incorrecto.
    if (reversi8_comprobar_movimiento(fila, columna)) {
        reversi8_mover_jugador(fila, columna);
        reversi8_mover_ia();
    }
    else {
        //Movimiento no valido
        GPIO_escribir(29,1,1);
    }
}

void controlar_alarmas() {
    static int alarma_activada = 0;
    if (numero_pulsados > 0 && alarma_activada == 0) { //Numero pulsados > 0 (algo requiere alarma)
        alarma_activada = 1;
        temporizador_alarma_periodica(RETARDO);
    }
    else if (numero_pulsados == 0 && alarma_activada == 1) {
        alarma_activada = 0;
        temporizador_desactivar_alarma();
    }
}

void gestionar_boton0(uint8_t interrupcion_boton) {

    static estado_boton_t estado = no_pulsado;

    if (interrupcion_boton == 0 && estado == no_pulsado) {
        return;
    }

    // ha habido interrupcion
    if (estado == no_pulsado) {
        estado = pulsado;
        numero_pulsados++;
    }
    else {
        //Si boton es 1 se ha dejado de pulsar
        if (!eint0_esta_pulsado()) {
            eint0_clear_nueva_pulsacion();
            estado = no_pulsado;
            numero_pulsados--;
        }
    }
}

void gestionar_boton1(uint8_t interrupcion_boton) {

    static estado_boton_t estado = no_pulsado;

    if (interrupcion_boton == 0 && estado == no_pulsado) {
        return;
    }

    // ha habido interrupcion
    if (estado == no_pulsado) {
        estado = pulsado;
        numero_pulsados++;
    }
    else {
        //Si boton es 1 se ha dejado de pulsar
        if (!eint1_esta_pulsado()) {
            eint1_clear_nueva_pulsacion();
            estado = no_pulsado;
            numero_pulsados--;
        }
    }
}

void gestionar_led() {
    static int estado = 0;
    GPIO_escribir(31, 1, estado);
    estado = !estado;
}

void gestionar_eventos() {
    Estado state = INICIO;
    uint8_t evento = 0;
    uint32_t data = 0;
    uint32_t time = 0;
    while (1) {
        controlar_alarmas();
        while (!nuevoEvento()) {
            if (state == INICIO) {
                PM_idle();
                // TODO: Hacerlo con PM_power_down();
            }
            else {
                PM_idle();
            }
        }
        siguienteEvento(&data, &evento, &time);
        switch (evento) {
            case EV_BOTON0: {
                gestionar_boton0(1);
                switch (state) {
                    case INICIO: {
                        //Se ha realizado un movimiento.
                        //A espera de confirmación temporal (3s) o por botón.
                        state = ACEPTAR;
                        numero_pulsados++;
                        leer_movimiento();
                        cuenta_atras = RETARDO;
                        break;
                    }
                    case ACEPTAR: {
                        //El usuario ha confirmado su movimiento. 
                        //Se coloca la ficha y turno de la IA.
                        aceptar_movimiento();
                        state = INICIO;
                        numero_pulsados--;
                        break;
                    }
                    case FIN: {
                        //Se ha terminado la partida y comienza una nueva.
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
						//El jugador ha pasado de turno.
                        if (!reversi8_mover_ia()) {
                            //Si la IA tambien pasa finaliza la partida.
                            state = FIN;
                        }
                        break;
                    }
                    case ACEPTAR: {
                        //Se cancela el movimiento realizado.
                        numero_pulsados--;
                        limpiar_casilla(fila, columna); //Se asegura de que se no se modifica la casilla
												state = INICIO;
                        break;
                    }
                    case FIN: {
                        //Se ha terminado la partida y comienza una nueva.
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
                        }
                        numero_pulsados--;
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
