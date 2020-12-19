#include <LPC210x.H>                       /* LPC210x definitions */
#include "../UART0.h" 
#include "../reversi8.h"
#include "../timer0.h"
#include "../RTC.h"
#include "../boton_eint0.h"
#include "../WT.h"
#include "../Power_management.h"

#if 0
int main (void) {
  uart0_init();
	temporizador0_iniciar();
	//temporizador0_parar();
	print("Hola que tal!!");
	jenu();
	print(mostrarTablero());
	while (1);
}


#include "../WT.h" 
#include "../UART0.h" 

int main (void) {
  unsigned int i,sec;
	char str[] = "P";

  uart0_init();                           /* Initialize Serial Interface   */
	RTC_init();
	str[0] = RTC_leer_segundos() % 10 + '0';
	print(str);
	WT_init(5);
	while (1);

}
#endif
