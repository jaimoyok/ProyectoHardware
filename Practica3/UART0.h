#ifndef __UART0__H
#define __UART0__H


#define READ_BUFFER_SIZE 10	//Tamaño buffer
#define WRITE_BUFFER_SIZE 256
void uart0_init (void);
void print (char* cadena);

#endif
