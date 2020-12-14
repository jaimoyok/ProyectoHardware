                  
#include <LPC210x.H> /* LPC210x definitions */
#include <stdint.h>
	
void PAbt_Handler_function(void) {}
void DAbt_Handler_function(void) {}
	
int32_t shared_var = 0;	

uint32_t __swi(0) clock_gettime(void);
uint32_t __SWI_0 (void) {
	while(1);
}

void __swi(0xFF) enable_isr (void);
void __swi(0xFE) disable_isr (void);
void __swi(0xFD) enable_isr_fiq(void);
void __swi(0xFC) disable_isr_fiq(void);

int main (void) {
	clock_gettime();
}

