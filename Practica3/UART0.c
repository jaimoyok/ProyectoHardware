#include <LPC210x.H>                     /* LPC21xx definitions               */
#include "UART0.h"

#define CR     0x0D

//Cursor para indicar la posicion en el buffer
static int offset = 0;
static volatile char read_buffer[READ_BUFFER_SIZE];

void uart0_isr(void) __irq {
	read_buffer[offset] = U0RBR;					// Se guarda el dato en el buffer
	offset = (offset + 1) % READ_BUFFER_SIZE;
	//generarEvento(datoLeido)
	
	VICVectAddr = 0;                      /* Acknowledge Interrupt						 */
}

void uart0_init (void)  {       				/* Initialize Serial Interface       */     
	PCONP |= 0x8; 												/* Enable PCUART0*/
	
	U0LCR = 0x83;     //1000 0011         /* 8 bits, no Parity, 1 Stop bit     */
	U0DLL = 97; 													/* 9600 Baud Rate @ 15MHz VPB Clock  */

	U0FCR = 0x1;													/* Enable FIFO											 */

	PINSEL0 = 0x5;                  			/* Enable RxD0 and TxD0							 */
            
  U0LCR = 0x03;                         /* DLAB = 0    											 */
	U0IER = 0x3;													/* Enable RDA and THRE 							 */

	VICVectAddr4 = (unsigned long) uart0_isr;

	// 0x20 bit 6 (UART0) enables vectored IRQs.
	VICVectCntl4 = 0x20 | 6;
  VICIntEnable = VICIntEnable | 0x40; // Enable UART0 Interrupt
	
}


/* implementation of putchar (also used by printf function to output data)    */
int sendchar (int ch)  {                 /* Write character to Serial Port    */

  if (ch == '\n')  {
    while (!(U0LSR & 0x20));
    U0THR = CR;                          /* output CR */
  }
  while (!(U1LSR & 0x20));
  return (U0THR = ch);
}


int getchar (void)  {                     /* Read character from Serial Port   */

  while (!(U0LSR & 0x01));

  return (U0RBR);
}
