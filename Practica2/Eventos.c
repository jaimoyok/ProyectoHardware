 #include "cola.h"
 #include "stdint.h"
 #include "Eventos.h"
 #include "boton_eint0.h"
 #include "boton_eint1.h"
 #include "timer0.h"
 #include "GPIO.h"
 #include "Power_management.h"

 typedef enum {
     no_pulsado = 0,
     pulsado = 1
 } estado_boton_t;

 const int  RETARDO = 50; //periodo de comprobacion

static volatile int pulsacion = 0;
static volatile int mover = 0;
static volatile int fila;
static volatile int columna;
 // sólo llamar si hay interrupción de boton
 // o estoy estado pulsado
 void actualizar_movimiento(){
     fila = GPIO_leer(0,3);
     columna = GPIO_leer(8,3);
 }

 int leer_move(){
     return mover;
 }

 int leer_fila(){
     return fila;
 }

  int leer_columna(){
     return columna;
 }

 void gestionar_boton(uint8_t interrupcion_boton) {

     static estado_boton_t estado = no_pulsado;

     if (interrupcion_boton == 0 && estado == no_pulsado) {
         return;
     }

     // ha habido interrupcion
     if(estado == no_pulsado) {
        estado = pulsado;
        numero_pulsaciones++;
        temporizador_alarma_periodica(RETARDO);
     } 
     else {
			 //leeemos para comprobar si se activa el boton
        int boton = GPIO_leer(16,1);
				//si esta a 1 es que ya se ha dejado de pulsar
        if(boton == 1) {
            eint1_clear_nueva_pulsacion();
            estado = no_pulsado;
            temporizador_desactivar_alarma();
        }
    }
}


void gestionar_led(){

}
 void gestionar_eventos()
 {
     while(nuevoEvento()) {
       uint8_t evento = 0;
       uint32_t data = 0;
       uint32_t time = 0;
       siguienteEvento(&data, &evento, &time);
         switch (evento) {
             case EV_BOTON:{
                if(data == 1){
                    mover = 1;
                    gestionar_boton1(1);
                    acualizarfichas();
                }
                else{
                     mover = 0;
                     gestionar_boton0(1);
                }

                 break;}
             case EV_TIMER0: {
                    gestionar_boton1(0); 
                    gestionar_boton0(0);
                    gestionar_led();
                    break; }
         }
				avanzar();
     }
 }

 int esperar_movimiento(){
     while(!pulsacion){
        PM_power_down();
        gestionar_eventos();
     }
    
 }