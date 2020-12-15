                  
#include <LPC210x.H> /* LPC210x definitions */
#include <stdint.h>
	
void PAbt_Handler_function(void) {}
void DAbt_Handler_function(void) {}
	
int32_t shared_var = 0;	

uint32_t __swi(0) clock_gettime();
uint32_t __SWI_0 (void) {
	return 3;
}
uint32_t __SWI_FF (){
	return 1;
}
uint32_t __SWI_FE (){
	return 1;
}
void __swi(0xFF) enable_isr (void);
void __swi(0xFE) disable_isr (void);
void __swi(0xFD) enable_isr_fiq(void);
void __swi(0xFC) disable_isr_fiq(void);


int main (void) {
	enable_isr();
	clock_gettime();
}

