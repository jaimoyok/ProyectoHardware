#include <LPC210x.H>                       /* LPC210x definitions */
#include "Power_management.h"
// Set the processor into power down state 
// The watchdog cannot wake up the processor from power down
void PM_power_down (void)  {
	//ponemos el procesador en modo power down
 	EXTWAKE = 3; 
	//PCONP |= 0x6;	//No duerme los timers
	PCON |= 0x02; 
	//vuelves a poner el procesador a 60MHz
	Switch_to_PLL();
}

void PM_idle(void) {
	//duermes el procesador sin parar los timers;
  	PCON |= 0x01;
}
