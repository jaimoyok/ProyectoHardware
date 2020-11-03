#include "GPIO.h"
#include <stdint.h>
#include <LPC210X.H>  

//Permite emplear el GPIO
void GPIO_iniciar(void) {
}

int GPIO_leer(int bitInicial, int numBits) {
    uint32_t aux = IOPIN;
    aux = aux << bitInicial;
    return (aux >> (32-numBits)) << numBits
}

void GPIO_escribir(int bitInicial, int numBits, int valor) {


}

//Los bits indicados se utilizaran como bits de salida.
void GPIO_marcar_entrada(int bitInicial, int numBits) {
    int aux = 0;
    for (int i = 0; i < 32){
        if (i >= bitInicial && i < bitInicial + numBits)aux += (0x1 << i);
    }   
    IOSET = aux;
}

//Los bits indicados se utilizaran como bits de salida
void GPIO_marcar_salida(int bitInicial, int numBits) {
       for (int i = 0; i < 32){
        if (i < bitInicial && i >= bitInicial + numBits)aux += (0x1 << i);
    }   
    IOSET = aux;
}