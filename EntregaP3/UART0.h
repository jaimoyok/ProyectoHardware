#ifndef __UART0__H
#define __UART0__H

//Tamaño suficientemente grande para la escritura
#define WRITE_BUFFER_SIZE 256

void uart0_init (void);
void print (char* cadena);

#endif
