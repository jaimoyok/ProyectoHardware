#ifndef __UART0__H
#define __UART0__H


#define MAX_SIZE 256	//Tama�o buffer

void uart0_init (void);
int sendchar (int ch) ;
int getchar (void);

#endif
