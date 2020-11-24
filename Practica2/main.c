#include "reversi8.h"            
#include <LPC210x.H>                       /* LPC210x definitions */
#include "timer0.h"
#include "Power_management.h"
#include "Eventos.h"
#include "GPIO.h"
#include "boton_eint1.h"
#include "boton_eint0.h"


// Nota: wait es una espera activa. Se puede eliminar poniendo el procesador en modo iddle. Probad a hacerlo
void wait (void)  {                         /* wait function */
  unsigned int i;

  i = temporizador0_leer(); // reads the number of previous timer IRQs
  while ((i + 10) != temporizador0_leer());              /* waits for 10 interrupts, i.e. 50ms */
}

/*int main (void) {
  unsigned int j;                         
	 
	eint0_init(); // activates EINT0 interrupts
	eint1_init(); // activates EINT0 interrupts
	GPIO_iniciar();
	// Nota la gesti�n del GPIO vosotros la debe�s hacer en GPIO.c no en el main o en el reversi
	GPIO_escribir(0,32,0);
	GPIO_marcar_salida(31,1);
	GPIO_marcar_entrada(14,1);
	GPIO_marcar_entrada(16,1);
	temporizador_alarma_periodica(50);
	// bucle para comprobar el funcionamiento del bot�n. El objetivo es comprobar que se lleva bien la cuenta de pulsaciones
	// con s�lo una interrupci�n EXTINT0 por pulsaci�n
	// en este proyecto no va a funcionar porque la interrupci�n se activa por nivel y no se ha a�adido la gesti�n necesaria para ue s�lo interrumpa una vez.
	temporizador0_iniciar();
	while (leer_pulsaciones() < 4){
		PM_idle();
		gestionar_eventos(); // de aqu� s�lo despertamos si hay pulsaci�n
		};	
	// bucle que realiza un blink de leds cada 50ms	   
 	// generates an interrupt every 0,05ms and increments timeval0
	GPIO_marcar_salida(16,8);
	temporizador0_empezar();
	while (1)  {                                 
    for (j = 16; j < 23; j++) { // Blink LED 0,1,2,3,4,5,6 
      // Nota la gesti�n del GPIO vosotros la debe�s hacer en GPIO.c no en el main o en el reversi
	  GPIO_escribir(j,1,1); // Turn on LED 
      wait();                                //  call wait function 
      GPIO_escribir(j,1,0);                               //Turn off LED 
    }
    for (j = 23; j > 16; j-- ) {  
      // Nota la gesti�n del GPIO vosotros la debe�s hacer en GPIO.c no en el main o en el reversi
	  GPIO_escribir(j,1,1);                               
      wait ();                                  				
      GPIO_escribir(j,1,0);                              
    }
  }
}*/


int main (void){ 	
 	iniciarOIreversi();
 	gestionar_eventos();
	return 0;
}
