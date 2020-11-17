 #include "cola.h"
 #include "stdint.h"
 #include "Eventos.h"
 #include "boton_eint0.h"
 #include "timer0.h"
 #include "GPIO.h"

 typedef enum {
     no_pulsado = 0,
     pulsado = 1
 } estado_boton_t;

 const int  RETARDO = 50; //periodo de comprobacion

static volatile int numero_pulsaciones = 0;
 // sólo llamar si hay interrupción de boton
 // o estoy estado pulsado

 int leer_pulsaciones(){
     return numero_pulsaciones;

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
            eint0_clear_nueva_pulsacion();
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
             case EV_BOTON:
                { gestionar_boton(1); break;}
             case EV_TIMER0:
                { gestionar_boton(0); gestionar_led(); break; }
         }
				avanzar();
     }
 }