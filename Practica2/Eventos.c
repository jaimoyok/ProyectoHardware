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
const int PERIODO = 12;  //numero de alarmas que se espera para hacer un movimiento
static volatile int numero_pulsaciones = 0;
static volatile int pulsacion = 0;
static volatile int mover = 0;
static volatile int fila;
static volatile int columna;
static volatile int cuenta_atras = PERIODO;
static volatile int aceptando = 0;
static volatile uint8_t *casilla;

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
    GPIO_marcar_entrada(16, 1);
    GPIO_marcar_entrada(14, 1);
    GPIO_marcar_entrada(0, 3);
    GPIO_marcar_entrada(8, 3);
    temporizador0_iniciar();
    temporizador1_iniciar();
    reversi8_iniciar();
}

void actualizar_movimiento() {
    fila = GPIO_leer(0, 3);
    columna = GPIO_leer(8, 3);
}

void gestionar_boton0(uint8_t interrupcion_boton) {

    static estado_boton_t estado = no_pulsado;

    if (interrupcion_boton == 0 && estado == no_pulsado) {
        return;
    }

    // ha habido interrupcion
    if (estado == no_pulsado) {
        estado = pulsado;
        numero_pulsaciones++;
    }
    else {
        //leeemos para comprobar si se activa el boton
        int boton = GPIO_leer(16, 1);
        //Si boton es 1 se ha dejado de pulsar
        if (boton == 1) {
            eint0_clear_nueva_pulsacion();
            estado = no_pulsado;
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
        numero_pulsaciones++;
    }
    else {
        //leeemos para comprobar si se activa el boton

        int boton = GPIO_leer(14, 1);
        //Si boton es 1 se ha dejado de pulsar
        if (boton == 1) {
            eint1_clear_nueva_pulsacion();
            estado = no_pulsado;
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
                        temporizador_alarma_periodica(60);
                        actualizar_movimiento();
                        cuenta_atras = RETARDO;
                        break;
                    }
                    case ACEPTAR: {
                        //El usuario ha confirmado su movimiento. 
                        //Se coloca la ficha y turno de la IA.
                        temporizador_desactivar_alarma();
                        reversi8_mover_jugador(fila, columna);
                        reversi8_mover_ia();
                    }
                    case FIN: {
                        //Se ha terminado la partida y comienza una nueva.
                        reversi8_iniciar();
                        state = INICIO;
                    }

                    default:
                        break;
                }
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
                        temporizador_desactivar_alarma();
                        state = INICIO;
                        break;
                    }
                    case FIN: {
                        //Se ha terminado la partida y comienza una nueva.
                        reversi8_iniciar();
                        state = INICIO;
                    }
                    default:
                        break;
                }
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
                            //Se confirma el movimoento realizado al concluir el tiempo.
                            temporizador_desactivar_alarma();
                            reversi8_mover_jugador(fila, columna);
                            reversi8_mover_ia();
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        avanzar();
    }
}

void esperar_movimiento() {
    while (!pulsacion) {
        PM_idle();
        gestionar_eventos();
    }
    actualizar_movimiento();
    pulsacion = 0;
    cuenta_atras = PERIODO;
}

void aceptar_movimiento(uint8_t *_casilla)
{
    casilla = _casilla;
    aceptando = 1;
    cuenta_atras = PERIODO;
    while (!pulsacion)
    {
        PM_idle();
        gestionar_eventos();
    }
    *casilla = 0;
}
