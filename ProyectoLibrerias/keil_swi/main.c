                  
#include <LPC210x.H> /* LPC210x definitions */
#include <stdint.h>
	
void PAbt_Handler_function(void) {}
void DAbt_Handler_function(void) {}
	
int32_t shared_var = 0;	
int  vectorIRQ = 0;
uint32_t __swi(0) clock_gettime();
uint32_t __SWI_0 (void) {
	//return temporizador1_leer();
	return 0;
}
void  __SWI_FF (){
	VICIntEnable = VICIntEnable | vectorIRQ;
}
void __SWI_FE (){
	vectorIRQ = VICIntEnable ;
	VICIntEnClr  = ~VICIntSelect & vectorIRQ;
}
void __SWI_FD(){
	VICIntEnable = VICIntEnable | vectorIRQ;
}
void __SWI_FC(){
	vectorIRQ = VICIntEnable ;
	VICIntEnClr  =  vectorIRQ;
}

void __swi(0xFF) enable_isr (void);
void __swi(0xFE) disable_isr (void);
void __swi(0xFD) enable_isr_fiq(void);
void __swi(0xFC) disable_isr_fiq(void);


int main (void) {
	enable_isr();
	clock_gettime();
	disable_isr();
	enable_isr_fiq();
	disable_isr_fiq();
}

