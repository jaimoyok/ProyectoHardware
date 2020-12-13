/******************************************************************************/
/* WATCHDOG.C: Watchdog timer Example for LPC2106                             */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/
                  
#include <LPC210x.H>                       /* LPC210x definitions */
#include "RTC.h" 
#include "WT.h" 
#include "Serial_port.h" 

int main (void) {
  unsigned int i,sec;

  init_serial();                           /* Initialize Serial Interface   */
	RTC_init();
	WT_init();
	while (1){
		sendchar(getchar());
		sendchar ('\n');
		//RTC_read_time() returns Seconds, Minutes, Hours, and Day of Week
		sec= RTC_read_time()&0x0000003F; // mask to read the seconds
		if (sec < 10) {
			sendchar ('0' +  sec);
			feed_watchdog();					   /* restart watchdog timer */
		}
		else if (sec < 20) {
			sendchar ('1');
			sendchar ('0' +  sec -10);}
		else {
			sendchar ('>');
			sendchar ('2');
			sendchar ('0');
		}
		sendchar ('\n');
	}

}
