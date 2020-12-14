#include <LPC210x.H>                       /* LPC210x definitions */
#include "Serial_port.h" 
void WT_init(int sec){
	// first, it checks for a previous watchdog time out  
	//WDTOF (bit 2)The Watchdog Time-Out Flag is set when the watchdog times out. This flag is cleared by software.
	if( WDMOD & 0x04 ) {					   /* Check for watchdog time out. El bit 2 se activa si se ha disparado el watchdog*/
    sendchar('W');
		sendchar('T');
		sendchar(' ');
		sendchar('r');
		sendchar('e');
		sendchar('s');
		sendchar('e');
		sendchar('t');
		sendchar('\n');
		WDMOD &= ~0x04;						   /* Clear time out flag           */
  }
	// Time out: Pclk*WDTC*4
	// valor m�nimo WTC= 256; valor m�ximo 2^32 (si escribes algo menos que FF, se escribe FF)

  WDTC  = 15000000 * 4 * sec;						   // Set watchdog time out value
  WDMOD = 0x03;                            /* Enable watchdog timer (bit 0) and reset (bit 1).  */   
	// se puede trabajar en dos modos: reset dando el valor 11 a los dos bits menos significativos; Int: dando 10 (en ese caso el WT genera una interrupci�n
	// Una vez se da un valor a estos bits �ya no se pueden cambiar a no ser que haya un reset externo, o la cuenta del WT acabe
	// Watchdog Timer Value register (WDTV) The WDTV register is used to read the current value of watchdog timer.
}
void feed_watchdog (void) {				   /* Reload the watchdog timer       */
// esta es la secuencia necesaria para que el watchdog se reinicialice. Si no se alimenta al WT antes de que termine la cuenta resetear� el sistema (si est� habilitado para ello)
// aunque se active el watchdog, este no har� nada hasta que no se le alimente por primera vez
//Important! Interrupts must be disabled during the feed sequence. An abort condition will occur if an interrupt happens during the feed sequence
  int aux = VICIntEnable;
	VICIntEnClr = 0xffffffff;	//Se desabilitan las interupciones
	WDFEED = 0xAA;						   
  WDFEED = 0x55;
  VICIntEnable = aux;	//Se habilitan las interrupciones
}
