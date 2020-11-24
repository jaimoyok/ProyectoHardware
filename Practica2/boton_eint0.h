#ifndef __BOTON_EINT0__H
#define __BOTON_EINT0__H
#include <stdint.h>
void eint0_init (void);
void eint0_clear_nueva_pulsacion(void);
int8_t eint0_esta_pulsado(void);


#endif
