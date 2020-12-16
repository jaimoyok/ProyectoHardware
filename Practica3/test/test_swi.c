#include "../SWI.h"
#include "../timer1.h"
int main (void){ 	
 	//iniciarOIreversi();
	enable_isr();
	clock_gettime();
	disable_isr();
	enable_isr_fiq();
	disable_isr_fiq();
	return 0;
}