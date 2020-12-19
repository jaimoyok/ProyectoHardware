#include <LPC210x.H>                       /* LPC210x definitions */

#define SEC_MASK 0x3F
#define MIN_MASK 0x3F00

void RTC_reset() {
	SEC = 0;
	MIN = 0;
}
void RTC_init(void){
	// prepara el RTC para que cuente los segundo (calculado para 60 MHz)
	PREINT = 0x726;
	PREFRAC = 0x700;
	// enable of the RTC
	CCR=0x01;
	
	RTC_reset();
}


int RTC_read_time(void){
	return CTIME0;
}

int RTC_leer_segundos(void){
	return RTC_read_time() & SEC_MASK;
}

int RTC_leer_minutos(void){
	return (RTC_read_time() & MIN_MASK) >> 8;
}


