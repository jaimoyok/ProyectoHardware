#include <LPC210x.H>                       /* LPC210x definitions */
#include "../UART0.h" 
#include "../reversi8.h"

int main (void) {
  uart0_init();
	print("Hola que tal!!");
	mostrarMenu();
	print(mostrarTablero());
	while (1);
}
