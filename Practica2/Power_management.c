#include <LPC210x.H>                       /* LPC210x definitions */
// Set the processor into power down state 
// The watchdog cannot wake up the processor from power down
void PM_power_down (void)  {
  EXTWAKE = 1; // EXTINT0 will awake the processor
	PCON |= 0x02; 
}
