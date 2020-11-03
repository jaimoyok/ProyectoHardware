#ifndef __GPIO_H__
#define __GPIO_H__

void GPIO_iniciar(void);
int GPIO_leer(int bitInicial, int numBits);
void GPIO_escribir(int bitInicial, int numBits, int valor);
void GPIO_marcar_entrada(int bitInicial, int numBits);
void GPIO_marcar_salida(int bitInicial, int numBits);

#endif