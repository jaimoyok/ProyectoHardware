#include <LPC210x.H>                       /* LPC210x definitions */
#include "UART0.h" 

int main (void) {
  uart0_init();                           /* Initialize Serial Interface   */
	while (1);
}