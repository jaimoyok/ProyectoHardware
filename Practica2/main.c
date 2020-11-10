                  
#include <LPC210x.H>                       /* LPC210x definitions */
#include "timer0.h"
#include "Power_management.h"
#include "boton_eint0.h"
#include "GPIO.h"

// Nota: wait es una espera activa. Se puede eliminar poniendo el procesador en modo iddle. Probad a hacerlo
void wait (void)  {                         /* wait function */
  unsigned int i;

  i = temporizador_leer(); // reads the number of previous timer IRQs
  while ((i + 10) != temporizador_leer());              /* waits for 10 interrupts, i.e. 50ms */
}

int main (void) {
  unsigned int j;                           /* LED var */
	 
	eint0_init(); // activates EINT0 interrupts
	GPIO_iniciar();
	// Nota la gesti�n del GPIO vosotros la debe�s hacer en GPIO.c no en el main o en el reversi
	GPIO_marcar_salida(16,8);	//Set LED pins as outputs
	GPIO_escribir(16,8,0); //Initialices the outputs to 0

	// bucle para comprobar el funcionamiento del bot�n. El objetivo es comprobar que se lleva bien la cuenta de pulsaciones
	// con s�lo una interrupci�n EXTINT0 por pulsaci�n
	// en este proyecto no va a funcionar porque la interrupci�n se activa por nivel y no se ha a�adido la gesti�n necesaria para ue s�lo interrumpa una vez.
	while (eint0_read_count() < 4){
		PM_power_down(); // de aqu� s�lo despertamos si hay pulsaci�n
		};	
// bucle que realiza un blink de leds cada 50ms	   
	temporizador_iniciar(); // generates an interrupt every 0,05ms and increments timeval0
	temporizador_empezar();
	while (1)  {                                  /* Loop forever */
    for (j = 16; j < 23; j++) { /* Blink LED 0,1,2,3,4,5,6 */
      // Nota la gesti�n del GPIO vosotros la debe�s hacer en GPIO.c no en el main o en el reversi
	  GPIO_escribir(j,1,1); /* Turn on LED */
      wait();                                  /* call wait function */
      GPIO_escribir(j,1,0);                               /* Turn off LED */
    }
    for (j = 23; j > 16; j-- ) { /* Blink LED 7,6,5,4,3,2,1 */
      // Nota la gesti�n del GPIO vosotros la debe�s hacer en GPIO.c no en el main o en el reversi
	  GPIO_escribir(j,1,1);                               /* Turn on LED */
      wait ();                                  					/* call wait function */
      GPIO_escribir(j,1,0);                              /* Turn off LED */
    }
  }
}


//int main (void) {
//  	
//  reversi8();
//	
//}
