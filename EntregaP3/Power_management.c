#include "Power_management.h"
#include <LPC210x.H> /* LPC210x definitions */

// Set the processor into power down state
// The watchdog cannot wake up the processor from power down
void PM_power_down(void) {
  // Se pone el procesador en modo Power Down
  EXTWAKE = 3;
  PCON |= 0x02;
  // Procesador a 60MHz
  Switch_to_PLL();
}

void PM_idle(void) {
  // Se duerme el procesador sin parar los timers;
  PCON |= 0x01;
}
