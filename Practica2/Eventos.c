 #include "cola.h"
 #include "stdint.h"
 #include "Eventos.h"
 #include "boton_eint0.h"
 #include "boton_eint1.h"
 #include "timer0.h"
 #include "timer1.h"
 #include "GPIO.h"
 #include "Power_management.h"

 typedef enum {
     no_pulsado = 0,
     pulsado = 1
 } estado_boton_t;

 const int  RETARDO = 250; //periodo de comprobacion
 const int  PERIODO = 12;    //numero de alarmas que se espera para hacer un movimiento
static volatile int numero_pulsaciones=0;
static volatile int pulsacion = 0;
static volatile int mover = 0;
static volatile int fila;
static volatile int columna;
static volatile int cuenta_atras= PERIODO;
static volatile int aceptando = 0;
int ** tablero;
 // sólo llamar si hay interrupción de boton
 // o estoy estado pulsado

void iniciarOIreversi(int **_tablero){
    //guardar la direccion de tablero
    tablero = _tablero;
  //activar perifericos
	 eint0_init();
  eint1_init();
  GPIO_iniciar();
	GPIO_marcar_salida(0,32);
	GPIO_escribir(24,8,0);
	GPIO_escribir(0,3,0);
	GPIO_escribir(8,3,0);
	GPIO_marcar_entrada(16,1);
	GPIO_marcar_entrada(14,1);
  GPIO_marcar_entrada(0,3);
  GPIO_marcar_entrada(8,3);
  temporizador0_iniciar();
  temporizador1_iniciar();
  temporizador_alarma_periodica(RETARDO);
}

 void actualizar_movimiento(void){
     fila = GPIO_leer(0,3);
     columna = GPIO_leer(8,3);
 }

int leer_pulsaciones(void){
  return numero_pulsaciones;
}

 int leer_move(void){
     return mover;
 }

 int leer_fila(void){
     return fila;
 }

  int leer_columna(void){
     return columna;
 }

 void parpadea(void){
     if(tablero[fila][columna] == 2)tablero[fila][columna] = 0;
     else tablero[fila][columna] == 2;
 }

 void gestionar_boton0(uint8_t interrupcion_boton) {

     static estado_boton_t estado = no_pulsado;

     if (interrupcion_boton == 0 && estado == no_pulsado) {
         return;
     }

     // ha habido interrupcion
     if(estado == no_pulsado) {
        estado = pulsado;
        numero_pulsaciones ++;
     }
     else {
			 //leeemos para comprobar si se activa el boton
		int boton = GPIO_leer(16,1);
				//si esta a 1 es que ya se ha dejado de pulsar
        if(boton == 1) {
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
    if(estado == no_pulsado) {
       estado = pulsado;
       numero_pulsaciones++;
    }
    else {
      //leeemos para comprobar si se activa el boton

       //int boton = GPIO_leer(16,1);
   int boton = GPIO_leer(14,1);
       //si esta a 1 es que ya se ha dejado de pulsar
       if(boton == 1) {
           eint1_clear_nueva_pulsacion();
           estado = no_pulsado;
       }
   }
}


void gestionar_led(void){
  static int estado = 0;
  GPIO_escribir(31,1,estado);
  estado = !estado;
}

void gestionar_eventos(void)
 {
     while(nuevoEvento()) {
       uint8_t evento = 0;
       uint32_t data = 0;
       uint32_t time = 0;
       siguienteEvento(&data, &evento, &time);
         switch (evento) {
             case EV_BOTON:{
               pulsacion = 1;
                if(data == 1){
                    mover = 0;
                    gestionar_boton1(1);
                }
                else{
                     mover = 1;
                     gestionar_boton0(1);
                }

                 break;}
             case EV_TIMER0: {
                  if(data == 0){
                    if(aceptando){
                        cuenta_atras--;
                        parpadear();
                    }
                    gestionar_boton1(0);
                    gestionar_boton0(0);
                    gestionar_led();
                    break; }
                  }

         }
         if(cuenta_atras == 0 & aceptando){
           pulsacion = 1;
         }
		avanzar();
     }

 }

void esperar_movimiento(void){
     while(!pulsacion){
        PM_idle();
        gestionar_eventos();
     }
     actualizar_movimiento();
     pulsacion = 0;
	 cuenta_atras = PERIODO;
 }
 
 void aceptar_movimiento(void){
    aceptando = 1;
    cuenta_atras = PERIODO;
    while(!pulsacion){
        PM_idle();
        gestionar_eventos();
     }
     tablero[fila][columna] = 0;
 }
