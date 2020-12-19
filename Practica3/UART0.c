#include <LPC210x.H>                     /* LPC21xx definitions               */
#include "UART0.h"
#include "Eventos.h"
#include "Power_management.h"
#include "SWI.h"

static volatile char read_buffer[READ_BUFFER_SIZE];

static int indice_escribir = 0;
static volatile char write_buffer[WRITE_BUFFER_SIZE];

void uart0_isr(void) __irq {
	char leido;
	
	int tipo = (U0IIR >> 1) & 0x7; 				// Bits [3:1] -> Interrupt Identification
	
	if (tipo == 0x1)											// THRE
		if (write_buffer[indice_escribir] == '\0') {	//Esta vacio el buffer de escritura
			indice_escribir = 0;
			write_buffer[indice_escribir] = '\0';
		}
		else {	//Hay datos pendientes de escribir
			U0THR = write_buffer[indice_escribir];
			indice_escribir++;
		}
	else if (tipo == 0x2) {								// RDA
		leido = U0RBR;
		U0THR = leido;		//Escribe el dato (Genera interrupcion THRE)
		//cola_guardar_eventos(EV_UART0, leido);
	}
			
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
	
	write_buffer[indice_escribir] = '\0';
	
}


/* Escribe una cadena en pantalla    */
void print (char* cadena)  {
	disable_isr();	//Desahabilitar para que no interrupa el timer y baje la interrupcion de uart tambien.
	int i = 0;
	int j = indice_escribir;
	while (cadena[i] != '\0') {
		write_buffer[j] = cadena[i];
		j++;
		i++;
	}
	write_buffer[j] = '\0';
	U0THR = '\n';
	while(write_buffer[indice_escribir] != '\0')
		PM_idle();
	
	enable_isr();
}
