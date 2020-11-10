 #include "cola.h"
 #include "stdint.h"
 #include "eventos.h"
 #include "boton_eint0.h"

 typedef enum {
     no_pulsado = 0,
     pulsado = 1
 } estado_boton_t;

 #define RETARDO 50; //periodo de comprobacion

static volatile int numero_pulsaciones = 0;
 // sólo llamar si hay interrupción de boton
 // o estoy estado pulsado
 void gestionar_boton(uint8_t interrupcion_boton) {

     static estado_boton_t estado = no_pulsado;
     static periodos = 0;

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
        int boton = GPIO_leer(16,1);
        if(boton == 0){
            eint1_clear_nueva_pulsacion();
            estado = no_pulsado;
        }
    }
}


void gestionar_led(){

}
 void gestionar_eventos()
 {
     while(avanza()) {
       int *evento;
       int *data;
       int *time;
       siguienteEvento(data,evento,time);
         switch (evento) {
             case EV_BOTON:
                { gestionar_boton(1); break;}
             case EV_TIMER0:
                { gestionar_boton(0); gestionar_led(); break; }
         }
     }
 }