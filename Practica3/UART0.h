#ifndef __UART0__H
#define __UART0__H


#define MAX_SIZE 256	//Tamaño buffer

void uart0_init (void);
int sendchar (int ch) ;
int getchar (void);

#endif
