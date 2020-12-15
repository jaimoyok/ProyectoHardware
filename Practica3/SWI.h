#ifndef __SWI__H
#define __SWI__H

#include <LPC210x.H> /* LPC210x definitions */
#include <stdint.h>

//TODO: Mover a timer
uint32_t __swi(0) clock_gettime();
uint32_t __SWI_0 (void) {
	return temporizador1_leer();
}

void __swi(0xFF) enable_isr (void);
void __swi(0xFE) disable_isr (void);
void __swi(0xFD) enable_isr_fiq(void);
void __swi(0xFC) disable_isr_fiq(void);

#endif