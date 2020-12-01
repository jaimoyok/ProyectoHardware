                  
#include <LPC210x.H> /* LPC210x definitions */
#include <inttypes.h>

void PAbt_Handler_function(void) {}
void DAbt_Handler_function(void) {}
	
int32_t shared_var = 0;

void __swi(0) increase_var(void);
void __SWI_0 (void) {
	shared_var++;
}

void __swi(0xFF) decrease_var (void);

int main (void) {

  decrease_var();
	
	increase_var();
	
	while(1);
}

