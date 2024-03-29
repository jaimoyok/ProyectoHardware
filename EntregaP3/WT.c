#include <LPC210x.H>                       /* LPC210x definitions */
#include "UART0.h" 
#include "SWI.h"

void feed_WT() {				   						/* Reload the watchdog timer       */
  disable_isr_fiq();									//Se desabilitan las interupciones
	WDFEED = 0xAA;						   
 	WDFEED = 0x55;
	enable_isr_fiq();										//Se habilitan las interrupciones
}
void WT_init(int sec){
	if( WDMOD & 0x04 )									/*El bit 2 se activa si se ha disparado el watchdog*/
		WDMOD &= ~0x04;										/* Clear time out flag*/

	// Time out: Pclk*WDTC*4
	WDTC  = (60000000 * sec) / 4; 			/*60Mhz -> 60M tics por segundo*/
	WDMOD = 0x03;                       /* Enable watchdog timer (bit 0) and reset (bit 1).  */  
	feed_WT();			 										/*Se alimenta para comenzar la cuenta*/
}


