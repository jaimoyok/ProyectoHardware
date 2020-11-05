#include "GPIO.h"
#include <stdint.h>
#include <LPC210X.H>  

char activado = 0;

//Permite emplear el GPIO
void GPIO_iniciar(void) {
    activado = 1;
}

int GPIO_leer(int bitInicial, int numBits) {
    uint32_t aux = -1;
    if(activado) {
        aux = IOPIN;
        aux = aux >> bitInicial; //Se eliminan los bits de la der.
        aux = aux << (32 - numBits) >> (32 - numBits); //Se eliminan los bits de la izq.
    }
    return aux;
}

void GPIO_escribir(int bitInicial, int numBits, int valor) {
    if (activado) {
        valor <<= 32 - numBits; //Elimina los bits sobrantes de valor 
        IOSET = valor >> (32 - bitInicial - numBits), //Escribe los bits de valor en la posicion dada
        IOSET;
    }
}

//Los bits indicados se utilizaran como bits de entrada.
void GPIO_marcar_entrada(int bitInicial, int numBits) {
    if(activado){
        int aux = 0;
        for (int i = bitInicial; i < bitInicial + numBits; ++i) {
            aux |= (1 << i); //Poner bit a 1
        }
        IODIR = aux;
    } 
}

//Los bits indicados se utilizaran como bits de salida
void GPIO_marcar_salida(int bitInicial, int numBits) {
    if(activado){
        int aux = 0xFFFFFFFF;
        for (int i = bitInicial; i < bitInicial + numBits; ++i) {
            aux &= ~(0x1 << i); //Poner bit a 0
        }   
        IODIR = aux;
    }
   
}