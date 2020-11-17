#include <LPC210x.H>                       /* LPC210x definitions */
#include "Power_management.h"
// Set the processor into power down state 
// The watchdog cannot wake up the processor from power down
void PM_power_down (void)  {
  EXTWAKE = 3; // 	 will awake the processor
	//PCONP |= 0x6;	//No duerme los timers
	PCON |= 0x02; 
	Switch_to_PLL();
}

void PM_idle(void) {
  PCON |= 0x01;
}
