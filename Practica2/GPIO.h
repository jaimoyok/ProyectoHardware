#ifndef __GPIO_H__
#define __GPIO_H__

GPIO_iniciar(void);
GPIO_leer(int bitInicial, int numBits);
GPIO_escribir(int bitInicial, int numBits, int valor);
GPIO_marcar_entrada(int bitInicial, int numBits);
GPIO_marcar_salida(int bitInicial, int numBits);

#endif