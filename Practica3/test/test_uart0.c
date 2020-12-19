#include <LPC210x.H>                       /* LPC210x definitions */
#include "../UART0.h" 
#include "../reversi8.h"
#include "../timer0.h"
#if 0
int main (void) {
  uart0_init();
	temporizador0_iniciar();
	//temporizador0_parar();
	print("Hola que tal!!");
	mostrarMenu();
	print(mostrarTablero());
	while (1);
}
#endif
